#ifndef _SERIAL_H_
#define _SERIAL_H_

int set_interface_attribs(int fd, int speed, int parity);

void set_blocking(int fd, int should_block);
 
#endif /* _SERIAL_H_ */
