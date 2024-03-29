#include "ds18b20.h"
// #include "init.h"
#include "delay.h"
void DS18B20_init(void)                            
{                                      
       onewire_reset();
       _delay_ms(100);
}             
float DS18B20_get_temperature(void)
{                                              
       unsigned char msb = 0x00;
       unsigned char lsb = 0x00;
       register float temp = 0.0; 
       onewire_reset();    
       onewire_write(skip_ROM);       
       onewire_write(convert_T);
       switch(resolution)  
       {                                                 
              case 12:
              {                                              
                     _delay_ms(750);
                     break;
              }               
              case 11:                                    
              {             
                     _delay_ms(375);
                     break;
              }          
              case 10:                            
              {                                
                     _delay_ms(188);  
                     break;
              }                                       
              case 9:                                  
              {                                               
                     _delay_ms(94);                
                     break;                           
              }                       
       }                 
       onewire_reset();
       onewire_write(skip_ROM);                
       onewire_write(read_scratchpad);
       lsb = onewire_read();
       msb = onewire_read();
       temp = msb;                          
       temp *= 256.0;
       temp += lsb;
       switch(resolution)  
       {                                 
              case 12:           
              {                                               
                     temp *= 0.0625;                
                     break;                           
              }            
              case 11:
              {             
                     temp *= 0.125;     
                     break;
              }               
              case 10:
              {           
                     temp *= 0.25;      
                     break;
              } 
              case 9:                                
              {                                
                     temp *= 0.5;       
                     break;     
              }                         
       } 
       _delay_ms(40);      
       return (temp);      
}