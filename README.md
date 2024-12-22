# MiniBike-Control
Research Project - MiniBike Control

### TODOs:
- [X] Explore MiniBike Control codes
    - [X] Understand LQR and PID controllers for longitudinal and steering control
    - [x] Explore how to reverse bike direction
    - [x] Explore how to use STM32 coding IDEs.
    - [x] STM32CubeIDE exploration
    - [ ] Understand Difference between the 3 directories provided for the bike
        - [x] 1: Control Codes with main file found in USER directory
        - [x] 2: Control Codes with main file not found in USER directory
        - [x] 3: Equipped with HAL Libraries (Hardware Abstraction Layer) which should facilitate the development of the code. But no control codes found!

- [X] Read papers
    - [X] Read & Understand Data-driven MPC for Linear Systems paper (paper 1)
        - [X] How to implement code?
          - Matlab codes available
    - [ ] Read paper for Robustness and Adaption (paper 2)
    - [X] Read Bicycle B.Sc. thesis
        - [X] See modelling & controller
    - [X] Read Survey of Bicycle modelling
    - [X] Translate and Read MiniBike Documentation

- [ ] Understand how to implement Data-driven MPC simulation for MiniBike
  - [ ] Use the codes from the Data-driven MPC for Linear Systems paper
  - [X] Find A and B matrices
    - [X] Measure Lambda (fork angle) and rtau
    - [ ] Compare models from
      - [X] Morlock B.Sc. thesis
      - [ ] Survey of Bicycle Modelling
      - [X] MiniBike Documentation
    - [ ]