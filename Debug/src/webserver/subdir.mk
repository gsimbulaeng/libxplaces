################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/webserver/xpClientSocket.cpp \
../src/webserver/xpServerSocket.cpp \
../src/webserver/xpSocket.cpp 

OBJS += \
./src/webserver/xpClientSocket.o \
./src/webserver/xpServerSocket.o \
./src/webserver/xpSocket.o 

CPP_DEPS += \
./src/webserver/xpClientSocket.d \
./src/webserver/xpServerSocket.d \
./src/webserver/xpSocket.d 


# Each subdirectory must supply rules for building sources it contributes
src/webserver/%.o: ../src/webserver/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -fPIC -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


