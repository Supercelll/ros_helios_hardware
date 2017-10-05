// Uses POSIX functions to send and receive data from a Maestro.
// NOTE: The Maestro's serial mode must be set to "USB Dual Port".
// NOTE: You must change the 'const char * device' line below.
 
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
 
#ifdef _WIN32
#define O_NOCTTY 0
#else
#include <termios.h>
#endif

/**
 * Gets the position of a Maestro channel.
 * See the "Serial Servo Commands" section of the user's guide.
 * @param fd
 * @param channel
 * @return
 */
int maestroGetPosition(int fd, unsigned char channel)
{
  unsigned char command[] = {0x90, channel};
  if(write(fd, command, sizeof(command)) == -1)
  {
    perror("error writing");
    return -1;
  }

  unsigned char response[2];
  if(read(fd,response,2) != 2)
  {
    perror("error reading");
    return -1;
  }

  return response[0] + 256*response[1];
}

/**
 * Sets the target of a Maestro channel.
 * See the "Serial Servo Commands" section of the user's guide.
 * The units of 'target' are quarter-microseconds.
 * @param fd
 * @param channel
 * @param target
 * @return
 */
int maestroSetTarget(int fd, unsigned char channel, unsigned short target)
{
  unsigned char command[] = {0x84, channel, target & 0x7F, target >> 7 & 0x7F};
  if (write(fd, command, sizeof(command)) == -1)
  {
    perror("error writing");
    return -1;
  }
  return 0;
}

/**
 * Sets the acceleration of a Maestro channel.
 * See the "Serial Servo Commands" section of the user's guide.
 * The speed limit is given in units of (0.25 μs)/(10 ms)
 * Speed of 0 is unlimited
 * @param fd
 * @param channel
 * @param target
 * @return
 */
int maestroSetSpeed(int fd, unsigned char channel, unsigned short target)
{
    unsigned char command[] = {0x87, channel, target & 0x7F, target >> 7 & 0x7F};
    if (write(fd, command, sizeof(command)) == -1)
    {
        perror("error writing");
        return -1;
    }
    return 0;
}

/**
 * Sets the speed of a Maestro channel.
 * See the "Serial Servo Commands" section of the user's guide.
 * The acceleration limit is a value from 0 to 255 in units of (0.25 μs)/(10 ms)/(80 ms)
 * Acceleration of 0 is unlimited
 * @param fd
 * @param channel
 * @param target
 * @return
 */
int maestroSetAccel(int fd, unsigned char channel, unsigned short target)
{
    unsigned char command[] = {0x84, channel, target & 0x7F, target >> 7 & 0x7F};
    if (write(fd, command, sizeof(command)) == -1)
    {
        perror("error writing");
        return -1;
    }
    return 0;
}

/**
 * Connexion de la maestro
 * @param device
 * @return
 */
int maestroConnect(const char * device){
//    const char * device = "/dev/ttyACM0";  // Linux
    int fd = open(device, O_RDWR | O_NOCTTY);
    if (fd == -1)
    {
      printf("Connection failed\n");
      perror(device);
      return 1;
    }

    // Aucune idée de si c'est important mais au cas où je laisse
//    #ifdef _WIN32
//        _setmode(fd, _O_BINARY);
//    #else
//        struct termios options;
//        tcgetattr(fd, &options);
//        options.c_iflag &= ~(INLCR | IGNCR | ICRNL | IXON | IXOFF);
//        options.c_oflag &= ~(ONLCR | OCRNL);
//        options.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
//        tcsetattr(fd, TCSANOW, &options);
//    #endif

    return fd;
}

//=========================
// EXEMPLE D'UTILISATION
//=========================
//int main()
//{
//  fd = maestroConnect("/dev/ttyACM0")
//  int position = maestroGetPosition(fd, 0);
//  printf("Current position is %d.\n", position);
//
//  int target = (position < 6000) ? 7000 : 5000;
//  printf("Setting target to %d (%d us).\n", target, target/4);
//  maestroSetTarget(fd, 0, target);
//
//  sleep(1);
//  target = 5000;
//  printf("Setting target to %d (%d us).\n", target, target/4);
//  maestroSetTarget(fd, 0, target);
//
//  close(fd);
//  return 0;
//}
