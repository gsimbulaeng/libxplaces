################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/udp/xpUDPService.cpp 

OBJS += \
./src/udp/xpUDPService.o 

CPP_DEPS += \
./src/udp/xpUDPService.d 


# Each subdirectory must supply rules for building sources it contributes
src/udp/%.o: ../src/udp/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -fPIC -DMACOS_X=1 -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


