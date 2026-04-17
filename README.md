#Project Name
Fault Diagnosis of Aircraft Aileron Systems Using an Event-Driven Hybrid Modeling Framework

#Overview
The project proposes a novel framework for diagnosing aileron faults using the abstraction capability of the Discrete Event System (DES) framework. The DES framework
uses the abstract behavior of a system for modeling, but it is a challenging task to implement in physical systems, as its state transitions are event-based. These
events need to be detected from the system sensor readings, which are usually continuous in nature. This project uses Thresholding for events that are universally
defined, such as the aileron going upwards or the aileron going downwards. But for other events which can't be discretized using a threshold alone due to the dynamic 
nature of the system, continuous data trends and pattern recognition can be used. For this purpose, this work employs a machine learning model to detect the force
related events. 
For implementation, this work has chosen to use an existing Simulink/Simscape-based aileron model.

#Features
1. DES model and diagnosis tool developed and implemented using Stateflow for the individual leadscrews of the aileron.
2. A dataset has been created with around 92,000 samples, including the commanded angle, measured angle, related force, and the type of force-related event.
3. A trained machine learning model using a "Decision Tree Classifier" based on the dataset for detecting the events.
4. Scopes showing the continuous time signals available from the system, the detected events, estimated states by the model, and the estimated status of the system.

#Installation
Must have Matlab Simulink installed, along with Simscape, and other related toolboxes.

#Usage
To run and visualize the project, follow these steps:
1. After opening MATLAB, open the Aileron_Actuator.prj file to open the project.
2. Once the project file opens, multiple tabs should open at the same time, along with the necessary scope views.
3. The "Electric" tab helps visualize the overview of the system along with the developed individual leadscrews, their related DES models, and diagnosers.
<img width="1920" height="1029" alt="View1" src="https://github.com/user-attachments/assets/02179998-1b63-4cd1-8793-03695167cd30" /> 
4. If the electric tab does not show up, you can navigate to the electric tab from the default sm_aileron_actuator tab (as shown in the figure below) by clicking the Aileron Actuator System and choosing the Electric variant. The aileron command angle can be changed by altering the time series values in the Repeating Sequence tool box named "Command Angle".
<img width="1920" height="1033" alt="VIew0" src="https://github.com/user-attachments/assets/a33870bb-100f-4ab0-855a-35e8766e8eaf" />
5. A fault can be injected by going into each leadscrews.
6. To inject a power fault, use the constant box at the bottom named Time_setting. Depending on the value set in seconds, the fault will be injected during the
simulation. To activate the power fault, the constant box below the time setting box has to be set to 1. If the value is set to 0, no power fault will be injected.
<img width="1920" height="1029" alt="VIew2" src="https://github.com/user-attachments/assets/93d38b3d-cb8f-4e2d-b426-c7691a4855cb" />
7. To inject a jamming fault into one of the leadscrews, select the intended leadscrew and open it. Click the lightning symbol below the Bearing Friction box, and the Properties dialog box should open. In the dialog box, go to the fault section and click the box next to the "Enable" option to enable the jamming fault. Type the time of injection in the "Trigger fault at time" field as shown in the figure below.
<img width="1920" height="1031" alt="view3" src="https://github.com/user-attachments/assets/cf4f0566-1fb6-465e-ae08-b176d85dca85" />
8. To simulate normal mode, make sure that none of these are enabled.
9. After the desired mode is selected, run the simulation using the "Run" button from the toolbar at the top.
    
 
