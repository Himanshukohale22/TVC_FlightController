1.IMU Driver Development 
	
	Hardware : 
		MPU6050
		STM32G491RE
		
	Software  :
		Kalman filter
		Complementary filter 
		filters to extract gyroscopic and accelerometer values using raw values 

	Real values in angles in degrees and accelaration in m/s^2 to use in 
	closed loop (use filters)

2.Barometer Driver Development 

	Hardware : 
		BMP280
		STM32G491RE
	
	Software : 
		I2C communication 
		
Altitude calculation for barometer at sea level: 
		
	Altitude (meters) = 44330×(1−(P0​P​)0.1903)
	p0- pressure at your current location (in **hPa** or **millibar**)
	p - pressure at sea level (typically **1013.25 hPa**, but it can vary)


3.SD card Data logging Development 
	
	Hardware :  
		MicroSD card Adaptor
		SD-Card
		STM32G491RE	
	
	Software : 
		SPI communication
		CSV file data store for IMU/BARO/TIMESTAMP/MOTORS-Readings/PyroChannel
		Text file for Serial Data logging
		
	How data will store ?
	Timestamp - 1 param
	Data of MPU6050 - 6 param
	Data of BMP - 2 param
	Data of Motors  - 2 param , 1 joint param
	Data of controller for motors - 4 param
	
	
	Description: Each computer power on time new file should create	
	\**FatFs** to access the SD card **like a file system** (FAT32)
	
	CSV file : 
Time stamp | Ax | Ay | Az | Pressure | Temp | Altitude | Motor 1 | Motor 2 | X | Y | Z | Theta
	Text file :
Time stamp of process start|stop | MPU Init | Baro Init | Motor Check | 

	Firmware Architecture 
	
	    [Your Sensor Data]
                ↓
	       Format as CSV
                ↓
	      sprintf() → "123,456,789\r\n" -> data
                ↓
           f_write() (FatFs)
                ↓
	      disk_write() → sector write
                ↓
	     SPI sends bytes to SD card
                ↓
		 SD card stores data in .txt/.csv file



4.Servo Motors Development 
	
	Hardware : 
		MG905 Servo motors 
		STM32G491RE
	
	Software :
		Timer PWM generation 
		Interrupt to control motor using commands
		
5.Parachute Deployment 

	Hardware :
		Parachute
		STM32G491RE
	Software:
		Digital for on/off
		Interrupt for trigger (Altitude and motion dependant)
	

6.Bootloader Customisation 
	
	1. Needed the customized bootloader to minimize power comsuption and use of unwanted peripherals 
	
	2. why normally Bootloaders of microcontroller customized when working on specific projects ??
- Answer is for 


Reference links  : 
SD card firmware : 
https://01001000.xyz/2020-08-09-Tutorial-STM32CubeIDE-SD-card/

STM32 debugging and Firmware dev :
https://deepbluembedded.com/stm32-ecosystem-development-environment-setup/

How to use MMC/SDC:
https://elm-chan.org/docs/mmc/mmc_e.html



-----------------------------Final software architecture--------------------------------------

Pseudo code for TVC dev

1. reading imu/bmp280 readings - 
2. if sensors is connected then read raw values, else error_handle - stop.
3. converting raw values into meaningful orientation values
4. complementary filter for imu and bmp values to fuse and get the roll/pitch/yaw values with altitude as in z-axis direction.
	1. getting z and az values for parachute deployment mechanism 
		1. kalman filter to fuse two sensors readings to get velocity and z values to determine apogee detection (ie current_vel <0 && previous_vel >0). - stop 
5. roll/pitch and theta for centre of body.
6. roll/pitch and theta for TVC centre (shifted at lower part of rocket).


typedef struct{
float gx,gy,gz;
float ax,ay,az;
}imu_data;

typedef struct{
float press;
float alti;
}baro_data;

error_handle()
	exit_system;

Initialize - bmp 
Initialize - imu 

if sensors== Not_ok
error_handle()

data_logger(imu_data,bmp_data){
	Fatfs initialization;
	sd_card_detection;
	create_csv_file;
	create header of data; manuall prinft()
	store_data;
	close_file;
}
loop...

// reading imu sensor values
int imu_sensors_values(){
imu_data.imu_values = raw_values;
}

//reading bmp sensors values; 
int bmp_sensor_values(){
baro_data.press =  int(buff_values);
}

//storing data in sd card
data_logger();

// converting raw values into 
int converting_raw_values(){
//converting raw_values into meaningful orientation and accelaraiton ;
//implementing complementary filter to fuse imu and barometer 

fused_roll  = COMPLEMENTARY_FILTER(gyro_roll, accel_roll)
fused_pitch = COMPLEMENTARY_FILTER(gyro_pitch, accel_pitch)
fused_yaw   = COMPLEMENTARY_FILTER(gyro_yaw, mag_yaw)   // if available
fused_altitude = altitude  // could also be smoothed
 
}

void para_deploy(altitude,imu_data){
//implementing kalman filter for sensor fusion between imu az and altitude
	for velocities calculation;
	if (current_vel< 0 && previous_vel>0){
		apogee_detect
		pyro_active;
		pwm_control_servo(); stop pwm at both servo 
		}
}

 pwm_control_servo(){
 fused_values(fused_roll, fused_pitch, fused_yaw);
	 thera = (servo1_x , servo2_y);
	 x = servo1_x;
	 y = servo2_y;
//control algorithms for servo position mapping with x,y,theta at origin;

 }

end loop...

     +------------------+      +----------------+      +--------------------+
     |  IMU Sensor      | ---> | Orientation    | ---> | Error Calculation  |
     |  (e.g., BNO085)  |      | (Pitch, Yaw)   |      | vs Desired Attitude|
     +------------------+      +----------------+      +--------------------+
                                                              |
                                                              v
                                                +-----------------------------+
                                                |  PID Controller (Pitch/Yaw)|
                                                +-----------------------------+
                                                              |
                                                              v
                          +-------------------------------------------+
                          |  Correction Angle Output (e.g. ±15 deg)   |
                          +-------------------------------------------+
                                                |
                                                v
                       +--------------------------------------------------+
                       |  Mapping to Servo PWM (e.g. 1000–2000 µs)       |
                       |  Formula: PWM = 1500 + (Angle / Max) * 500      |
                       +--------------------------------------------------+
                                                |
                                                v
                              +------------------------------+
                              |   Servo PWM Output to Gimbal |
                              |  (Pitch Servo, Yaw Servo)    |
                              +------------------------------+
                                                |
                                                v
                           +---------------------------------------+
                           |   Gimbal tilts nozzle to redirect     |
                           |     thrust for pitch/yaw correction   |
                           +---------------------------------------+


Electronics Parachute deployment system: 
using Apogee algoriths :
 utilized an Altimeter and Accelerometer to deploy the rocket’s parachutes without failure
 1. Velocities
 2. Acceleration 
 3. Altitude

Apogee detection: 
"Zero-g force" (or **zero apparent acceleration**) means that the accelerometer onboard the rocket **measures near 0 g** (acceleration due to gravity) — because it is in **free fall**.

This is called the **“zero-g condition”** — a strong indicator of apogee.

At moment when vertical velocity becomes zero and only one force is applied on rocker is grevitation in z direction which will be zero on imu readings called free fall systems


magnitude of acceleration vector: 

It uses the **Euclidean norm** (Pythagorean theorem in 3D) to calculate the **total acceleration**:

total_accel=x2+y2+z2\text{total\_accel} = \sqrt{x^2 + y^2 + z^2}total_accel=x2+y2+z2

if (total_accel =< 0){
free_fall;
}

