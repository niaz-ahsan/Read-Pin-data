/**
 ******************************************************************************
 Problem statement: Turn LD4(Green) based on PA0 on/off
 ******************************************************************************
 */

#include <stdint.h>


#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

void turn_on_LED(uint32_t volatile *RCC_AHB1, uint32_t volatile *GPIO_D_mode, uint32_t volatile *GPIO_D_output_data) {
	// setting 3rd bit of RCC Clock register to enable GPIO D peripheral
	*RCC_AHB1 |= (1 << 3);

	// 25,24 bit position of GPIO D Mode Register should be 0 and 1.
	// First, clear 25 bit position
	*GPIO_D_mode &= ~(1 << 25);
	// set 24 bit position
	*GPIO_D_mode |= (1 << 24);

	// setting 12 bit position of GPIO D o/p data, controlled by PD12
	*GPIO_D_output_data |= (1 << 12);
}

void turn_off_LED(uint32_t volatile *GPIO_D_output_data) {
	// clearing 12 bit position of GPIO D o/p data, controlled by PD12
	*GPIO_D_output_data &= ~(1 << 12);
}

int main(void)
{
	uint32_t volatile *GPIO_D_mode = (uint32_t*) 0x40020C00; // GPIO D Mode Register Address
	uint32_t volatile *GPIO_D_output_data = (uint32_t*) 0x40020C14; // GPIO D O/P Data Register Address

	// enable RCC AHB1 peripheral reset register for GPIO A
	uint32_t volatile *RCC_AHB1 = (uint32_t*) 0x40023830;
	*RCC_AHB1 |= (1 << 0); // enabling 0th bit for GPIO A

	// GPIO A port mode register address
	uint32_t volatile *GPIOA_port_mode_reg = (uint32_t*) 0x40020000;
	// 0,1 bit position should be cleared for setting it into input mode
	*GPIOA_port_mode_reg &= ~(3 << 0);

	// GPIO A Peripheral i/p data register address for PA0
	uint32_t volatile *GPIOA_input_data_reg = (uint32_t*) 0x40020010;

	while(1) {
		uint8_t pin_status = (uint8_t) *GPIOA_input_data_reg & 0x01;

		if(pin_status) {
			// turn on LED
			turn_on_LED(RCC_AHB1, GPIO_D_mode, GPIO_D_output_data);
		} else {
			// turn off LED
			turn_off_LED(GPIO_D_output_data);
		}
	}


}
