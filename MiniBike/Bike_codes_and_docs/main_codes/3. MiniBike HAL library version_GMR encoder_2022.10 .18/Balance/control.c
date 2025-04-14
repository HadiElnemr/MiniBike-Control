#include "control.h"
#include "math.h"

/**************************************************************************
Author: WHEELTEC
Website: http://shop114407458.taobao.com/
**************************************************************************/

uint8_t COUNT;

int Voltage_Temp, Voltage_Count, Voltage_All;  // Battery voltage variables
float BalancePoint_Offset = 0;                 // Offset for balance point
float LeftControl = 3.0f;                      // Left turn control factor
float RightControl = 3.0f;                     // Right turn control factor
float delta = 0, delta_dot = 0, last_delta_dot = 0, last_delta_dot_2 = 0;
// float k1 = -92.2973, k2 = -8.6746, k3 = 10.5355; // LQR coefficients Experiment #2
// float k1 = -103.4593, k2 = -9.7400, k3 = 8.7622; // LQR coefficients for fork angle model (75 degrees) (with negative sign for velocity) Experiment #1
//float k1 = -1.1471, k2 = -5.50458, k3 = -0.851251;  // For some data
//float k1 = -1.33968, k2 = -8.06395, k3 = -4.56877;  // For 1st stable data
//float k1 = -236.863, k2 = -3.14333, k3 = 35.2826;  // For some stable sys

// float k1 = -83.5311, k2 = -21.2345, k3 = -6.34192; // Data generation state-feedback T=5, eps=0.01, (not working)
// float k1 = -103.662, k2 = -22.3076, k3 = -7.67324; // Data generation state-feedback T=40, eps=0.001, (not working)
//float k1 = -106.993, k2 = -11.6399, k3 = 7.1795; // Data generation state-feedback T=40, eps=0.001, x_d0 [0.5,-0.1,0.1] (working)

// float k1 = -96.5329, k2 = -10.0665, k3 = 10.2872; // Data generation state-feedback T=40, eps=0.001, x_d0 [0.5,-0.1,0.1] (No Fork Angle)

// float k1 = -109.822, k2 = -18.8359, k3 = -2.2228; // Data generation state-feedback T=40, eps=0.001, x_d0 [0.5,-0.1,0.1] (with Fork Angle)

// After adding appropriate Sx and Su: -93.5741 -10.4275 9.23108 No fork
//                                     -93.8783 -9.99457 9.93788
//                                     -93.3355 -10.1406 9.54074
//                                     -93.7508 -10.1583 9.66352

// After adding appropriate Sx and Su: -111.804 -14.0193 4.06108 With 70 degrees fork (bad but slightly better)
//                                     -111.941 -15.8187 1.96986 (Did not work, bike falls in few seconds)
//                                     -112.041 -13.8409 4.32565
//                                     -111.901 -15.3078 2.62117

// After adding appropriate Sx and Su: -108.783 -16.4302 1.33409 With 75 degrees fork
//                                     -108.529 -17.2309 0.18906
//                                     -107.542 -19.1009 -2.61613
//                                     -107.88  -13.7282 4.66033

// float k1 = -93.7508, k2 = -10.1583, k3 = 9.66352; // Data generation state-feedback T=40, eps=0.001, x_d0 [0.5,-0.1,0.1] (No Fork Angle) Experiment #4
// float k1 = -111.804, k2 = -14.0193, k3 = 4.06108; // Data generation state-feedback T=40, eps=0.001, x_d0 [0.5,-0.1,0.1] (with Fork Angle 70) Experiment #3
float k1 = -107.88, k2 = -13.7282, k3 = 4.66033; // Data generation state-feedback T=40, eps=0.001, x_d0 [0.5,-0.1,0.1] (with Fork Angle 75) Experiment #3


int ki = 4, kp = 16, kd = 4;                   // PID coefficients
int u;                                         // PWM output variable

/**************************************************************************
Function: EXTI interrupt handler for balancing and control updates
**************************************************************************/
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == GPIO_PIN_12)
    {
        if (delay_flag == 1)
        {
            if (++delay_50 == 10)
            {
                delay_50 = 0;
                delay_flag = 0;
            }
        }

        if (COUNT < 3)
        {
            COUNT++;
            Get_Angle(Way_Angle);
            Voltage_Temp = Get_battery_volt();
            Voltage_Count++;
            Voltage_All += Voltage_Temp;

            if (Voltage_Count == 100)
            {
                Voltage = Voltage_All / 100;
                Voltage_All = 0;
                Voltage_Count = 0;
            }

            Led_Flash(100);
            SERVO_BIAS = (Get_Adc(Adjust_Ch, ADC_SAMPLETIME_1CYCLE_5) - 1024) / 12;

            if (Flag_Stop == 1)
            {
                BalancePoint_Offset = (5.06f / 1024.0f) * (Get_Adc(ZhongZhi_Ch, ADC_SAMPLETIME_1CYCLE_5) - 1024);
                BalancePoint_Offset = roundf(BalancePoint_Offset * 10) / 10.0f;
                balance_point = BalancePoint_Offset + balance_point_diff;
            }
            return;
        }

        COUNT = 0;
        Encoder = -Read_Encoder(2) / 38.4615f;
        Get_Angle(Way_Angle);
        phi_dot = Gyro_Balance / 16.4f;
        delta = (Servo - MID) / 3.65f;

        Key();

        if (Put_Down(phi, Encoder) == 1)
        {
            Flag_Stop = 0;
        }

        if (Turn_Off(phi, Voltage) == 0 && Flag_Stop == 0)
        {
            Get_RC();
            delta_dot = -(k1 * (phi - balance_point) + k2 * phi_dot + k3 * delta);
            delta_dot *= 100;
            delta_dot = (int)delta_dot;

            u = ki * delta_dot + kp * (delta_dot - last_delta_dot) + kd * (delta_dot - 2 * last_delta_dot + last_delta_dot_2);
            // add noise to u
//            u += (rand() % 100000) - 50000;
            Servo += u / 10000;

            if (Servo < (MID - 130))
                Servo = MID - 130;
            if (Servo > (MID + 130))
                Servo = MID + 130;

            last_delta_dot_2 = last_delta_dot;
            last_delta_dot = delta_dot;

            Motor = Incremental_PI_A(Encoder, -60);
            Pwm_Limit();
            Set_Pwm(-Motor);
        }
        else
        {
            Set_Pwm(0);
        }
    }
}

/**************************************************************************
Function: Reads the control commands from the app
**************************************************************************/
void Get_RC(void)
{
    static uint8_t TurnFlag = 0;
    static float turn;
    static uint8_t Last_L_Flag, Last_R_Flag;
    static uint8_t updateTime;
    static uint8_t WaitTime;

    if ((Last_L_Flag && Flag_Right) || (Last_R_Flag && Flag_Left))
    {
        if (++WaitTime < 50)
            return;
        else
            WaitTime = 60;
    }
    else
        WaitTime = 0;

    if ((Flag_Right || Flag_Left) && TurnFlag == 0)
    {
        turn = balance_point;
        TurnFlag = 1;
    }

    if (Flag_Left)
        balance_point = turn + LeftControl;
    else if (Flag_Right)
        balance_point = turn - RightControl;

    if (Flag_Right == 0 && Flag_Left == 0 && TurnFlag == 1)
    {
        balance_point = turn;
        TurnFlag = 0;
    }

    if (++updateTime > 50)
    {
        Last_L_Flag = Flag_Left;
        Last_R_Flag = Flag_Right;
        updateTime = 0;
    }
}

/**************************************************************************
Function: Incremental PI control for motor speed
**************************************************************************/
int Incremental_PI_A(int Encoder, int Target)
{
    static int Pwm, Bias, Last_bias, Last_bias_2;
    Bias = Encoder - Target;
    Pwm += 50 * (Bias - Last_bias) + 5 * Bias + 60 * (Bias - 2 * Last_bias + Last_bias_2);

    if (Pwm > 7200)
        Pwm = 7200;
    if (Pwm < -7200)
        Pwm = -7200;

    Last_bias_2 = Last_bias;
    Last_bias = Bias;

    return Pwm;
}

/**************************************************************************
Function: Sets PWM for motor control
**************************************************************************/
void Set_Pwm(int motor)
{
    if (motor > 0)
    {
        PWMA1 = 7200;
        PWMA2 = 7200 - motor;
    }
    else
    {
        PWMA2 = 7200;
        PWMA1 = 7200 + motor;
    }

    SERVO = Servo + SERVO_BIAS;
}

/**************************************************************************
Function: Limits PWM output
**************************************************************************/
void Pwm_Limit(void)
{
    int Amplitude = 6900;
    if (Motor < -Amplitude)
        Motor = -Amplitude;
    if (Motor > Amplitude)
        Motor = Amplitude;
}

/**************************************************************************
Function: Checks and adjusts key input for state transitions
**************************************************************************/
void Key(void)
{
    uint8_t tmp;
    static uint8_t flag = 0;
    tmp = click_N_Double(50);

    if (tmp == 1)
    {
        if (flag == 0)
        {
            if (Flag_Stop == 1 && (phi >= -10 && phi <= 10))
                Flag_Stop = 0;
        }
        else
            Flag_Stop = 1;

        if (Flag_Stop == 1)
            flag = 0;
        else
            flag = 1;
    }

    if (tmp == 2)
        Flag_Show = !Flag_Show;
}

/**************************************************************************
Function: Checks for system shutdown conditions
**************************************************************************/
uint8_t Turn_Off(float angle, int voltage)
{
    uint8_t temp;
    if (angle < -40 || angle > 40 || Flag_Stop == 1 || voltage < 710 || Roll < -20)
    {
        temp = 1;
        SERVO = MID + SERVO_BIAS;
        Flag_Stop = 1;
    }
    else
    {
        temp = 0;
    }
    return temp;
}

/**************************************************************************
Function: Gets the current angle using the selected method
**************************************************************************/
void Get_Angle(uint8_t way)
{
    if (way == 1)
    {
        Read_DMP();
        phi = Pitch;
        Gyro_Balance = gyro[1];
    }
}

/**************************************************************************
Function: Checks if the robot is balanced or tipped
**************************************************************************/
int Put_Down(float angle, int encoder)
{
    static uint8_t flag, count;

    if (flag == 0)
    {
        if (angle > (balance_point - 10) && angle < (balance_point + 10) && encoder < 2 && encoder > -2 && Flag_Stop == 1)
            flag = 1;
    }

    if (flag == 1)
    {
        if (++count > 50)
        {
            count = 0;
            flag = 0;
        }

        if (encoder < -8)
        {
            flag = 0;
            return 1;
        }
    }

    return 0;
}
