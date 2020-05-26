# ESCControlRPMCounter
Engine Speed Control with RPM counter for DAF vehicles.
### Implementation
- For this Project I used Atmel ATMEGA16A  MCU. RPM signal is comming from the central control unit. Signal is 6VDC square wave. By measuring frequency and real RPM, I concluded that the ratio of RPMs vs Frequency of square wave signal is five times smaller than the RPMS. For example, the idling RPMs are 700RPM, and the the frequency in this case is 140Hz. When the engine speed is 1000RPM, frequency is 200Hz, so the ratio is realiy 5.
## Measurements
- RPM is measured through 16BIT timer T1, and the value is sent to the 1602A LCD display using LCD library.
# ESC functionallity 
- Now a little about Engine Speed Control functionallity on DAF vehicle... To activate engine speed control, you must send an active high signal(but in this case, this is not +5VDC, but +12VDC and above) to the determined pin on central control unit for this functionality. For the Engine Speed Control to be active, AH signal must always be present, when AH signal is not present, functionality is not there.
- Next, for the rising of the engine speed(RPMS), we need to send pulse signal, again AH(also +12VDC and above). I choose the pulses around 200ms, this is enough that cental control unit can recognise the signal and to activate the functionality. By diagnostic tool of DAF, this pulse is set to raise the RPMs by 25.
- Now, we need also the droping engine speed functionality. For this AH(+12VDC and above) pulse signal activate the functionality in central control unit. Again pulse is set to 200ms, and by diagnostic tool of DAF, we set the RPM drop to be 25rpm. 
- This would be enough functionalities, but on older DAF vehicles, there is special software that remembers the last value of engine speed during Engine Speed Control active functionality. So, if for any reason, you shut down ESC functionality, for example @1500RPM, next time that you activate ESC functionality, you will have the last remembered RPM value, in this case 1500RPM. This is not good, for various reasons, I will not come into that.
- To prevent this, I added 4th functionality. That is when you activate STOP functionality(ESC OFF), frstly there is 50 pulses of 50ms at rpm drop, so that this rpm drop occurs in 2.5sec. and then the ESC ON functionality is turned on. This is an RPM drop of 1250RPMs, that are in most applications sufficient..
## End 
That would be in short, functionallity of this code. 
