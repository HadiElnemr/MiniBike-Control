# MiniBike-Control

This is a research project that was done at IST - Stuttgart University as part of my Infotech Master's studies.
The title of the project is "Implementation of a Data-driven State-feedback Controller for a Self-stabilising bicycle".
The purpose of this project is to test the data-driven min-max MPC developed in [1] on a model-scale bicycle, MiniBike, from WHEELTEC company.

# Setup

The project uses Matlab with the following toolboxes:
- Yalmip

The MiniBike  is controlled via an STM32 microcontroller, which is connected to a PC via USB. The controller is implemented in C and uses the STM32 HAL library.

Softwares used:
- STM32CubeIDE
- FlyMCU

# Usage
To run the matlab files,

# References

[1] Yifan Xie, Julian Berberich, and Frank Allgöwer. “Data-Driven Min-Max MPC
for Linear Systems”. In: 2024 American Control Conference (ACC). 2024, pp. 3184–
3189. doi: 10.23919/ACC60939.2024.10644295.
[2] Yifan Xie, Julian Berberich, and Frank Allgöwer. “Data-Driven Min-Max MPC
for Linear Systems: Robustness and Adaptation”. In: arXiv preprint arXiv:2404.19096
(2024). url: https://arxiv.org/abs/2404.19096.
[3] Karl J. Åström, Richard E. Klein, and Anders Lennartsson. “Bicycle Dynamics
and Control”. In: IEEE Control Systems Magazine 25.4 (2005), pp. 26–47. doi:
10.1109/MCS.2005.1499389.