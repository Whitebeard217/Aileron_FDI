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
1.
