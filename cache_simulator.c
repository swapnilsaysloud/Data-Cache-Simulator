#include <stdio.h>

#include <math.h>

#define KEY_BASE 0xff200050 // base address of pushbutton keys
#define KEY0_MASK 0x1
#define KEY1_MASK 0x2
#define KEY2_MASK 0x4
#define KEY3_MASK 0x8
// VGA display resolution
#define VGA_WIDTH 320
#define VGA_HEIGHT 240

// VGA colors
#define VGA_BLACK 0x0000
#define VGA_WHITE 0xFFFF
#define VGA_GREEN 0x0F00
#define VGA_BLUE 0x001F
#define VGA_RED 0xF000
int cache_size = 0;

int block_size = 0;

int hits = 0;

void
write_pixel(int x, int y, short colour) {

  volatile short * vga_addr =
    (volatile short * )(0x08000000 + (y << 10) + (x << 1));

  * vga_addr = colour;

}
void

clear_screen() {

  int x, y;

  for (x = 0; x < 320; x++) {

    for (y = 0; y < 240; y++) {

      write_pixel(x, y, 0);

    }

  }

}

int
modulo(int a, int b) {

  int res = a;

  while (res > b)

  {

    res = res - b;

  }

  return res;

}

void
draw_rectangle(int center_x, int center_y, int length, int breadth,
  short colour) {

  int x, y;

  int left = center_x - (length / 2);

  int right = center_x + (length / 2);

  int top = center_y - (breadth / 2);

  int bottom = center_y + (breadth / 2);

  for (x = left; x < right; x++) {

    for (y = top; y < bottom; y++) {

      write_pixel(x, y, colour);

    }

  }

}

void
draw_rect(int center_x, int center_y, int length, int breadth, short color) {

  int x, y;

  // Calculate the starting position of the rectangle
  int start_x = center_x - (length / 2);

  int start_y = center_y - (breadth / 2);

  // Loop through each pixel and draw the rectangle
  for (x = start_x; x < start_x + length; x++) {

    for (y = start_y; y < start_y + breadth; y++) {

      write_pixel(x, y, color);

    }

  }

}

void
draw_rect_boundary_mine(int x, int y, int l, int b) {

  int xmin = x - b / 2;

  int xmax = x + b / 2;

  int ymax = y + l / 2;

  int ymin = y - l / 2;

  for (int x = xmin; x < xmax; x++)

  {

    write_pixel(x, ymin, 0xFFFFFF);

    write_pixel(x, ymax, 0xFFFFFF);

  }
  for (int y = ymin; y < ymax; y++)

  {

    write_pixel(xmin, y, 0xFFFFFF);

    write_pixel(xmax, y, 0xFFFFFF);

  }

}
void

print_rect_boundary(int center_x, int center_y, int length, int breadth,
  short color) {

  int x_min = center_x - (length / 2);

  int x_max = center_x + (length / 2);

  int y_min = center_y - (breadth / 2);

  int y_max = center_y + (breadth / 2);

  // iterate through the rectangle coordinates
  for (int x = x_min; x <= x_max; x++) {

    write_pixel(x, y_min, color); // top side
    write_pixel(x, y_max, color); // bottom side
  }
  for (int y = y_min + 1; y < y_max; y++) {

    write_pixel(x_min, y, color); // left side
    write_pixel(x_max, y, color); // right side
  }
}
void

write_char(int x, int y, char c) {

  // VGA character buffer
  volatile char * character_buffer = (char * )(0x09000000 + (y << 7) + x);

  * character_buffer = c;

}
void

write_int(int x, int y, int num) {

  char str[20];

  sprintf(str, "%d", num); // convert integer to string

  int i = 0;

  while (str[i] != '\0') {

    write_char(x + i, y, str[i]);

    i++;

  }

}

// Function to write a string to the VGA display
void
write_string(int x, int y, char * str) {

  int i = 0;

  while (str[i] != '\0') {

    write_char(x + i, y, str[i]);

    i++;

  }

}

// Function to draw a box on the VGA display
void
draw_box(int x1, int y1, int x2, int y2, short color) {

  int x, y;

  for (x = x1; x <= x2; x++) {

    for (y = y1; y <= y2; y++) {

      write_pixel(x, y, color);

    }

  }

}

void
homescreen() {

  // Clear the screen
  clear_screen();

  // Draw the top and bottom bars
  draw_box(0, 0, VGA_WIDTH - 1, 29, VGA_BLUE);

  draw_box(0, VGA_HEIGHT - 30, VGA_WIDTH - 1, VGA_HEIGHT - 1, VGA_BLUE);

  draw_box(31, 30, VGA_WIDTH - 31, 60, VGA_RED);

  draw_box(31, VGA_HEIGHT - 60, VGA_WIDTH - 31, VGA_HEIGHT - 31, VGA_RED);

  draw_box(0, 30, 29, VGA_HEIGHT - 31, VGA_BLUE);

  draw_box(VGA_WIDTH - 30, 30, VGA_WIDTH - 1, VGA_HEIGHT - 31, VGA_BLUE);

  draw_box(31, 60, 60, VGA_HEIGHT - 61, VGA_RED);

  draw_box(VGA_WIDTH - 60, 60, VGA_WIDTH - 31, VGA_HEIGHT - 61, VGA_RED);

  char * s = "Welcome to cache data simulator";

  write_string(25, 25, s);

  char * p = "Press Key0 to continue";

  write_string(30, 28, p);

}

int

main() {

  volatile int * KEY_ptr = (int * ) KEY_BASE;

  homescreen();

  while (1)

  {

    if ( * KEY_ptr & KEY0_MASK) // if KEY0 is pressed
    {

      while ( * KEY_ptr & KEY0_MASK); // wait for KEY0 to be released
      break;

    }

  }

  int zz;

  for (zz = 0; zz < 33; zz++)

  {

    write_char(25 + zz, 25, 0);

  }

  for (zz = 0; zz < 23; zz++)

  {

    write_char(30 + zz, 28, 0);

  }

  clear_screen();

  int l = 0;

  print_rect_boundary(160, 40, 250, 40, 0xF800);

  char * hw = "Press KEY 0 FOR Cache Size - 64B";

  int x = 140;

  while ( * hw) {

    write_char(x, 10, * hw);

    x++;

    hw++;

  }

  print_rect_boundary(160, 90, 250, 40, 0xF800);

  hw = "Press KEY 1 FOR Cache Size - 128B";

  x = 140;

  while ( * hw) {

    write_char(x, 23, * hw);

    x++;

    hw++;

  }

  print_rect_boundary(160, 140, 250, 40, 0xF800);

  hw = "Press KEY 2 FOR Cache Size - 256B";

  x = 140;

  while ( * hw) {

    write_char(x, 35, * hw);

    x++;

    hw++;

  }

  print_rect_boundary(160, 190, 250, 40, 0xF800);

  hw = "Press KEY 3 FOR Cache Size - 512B";

  x = 140;

  while ( * hw) {

    write_char(x, 45, * hw);

    x++;

    hw++;

  }

  while (1)

  {

    if ( * KEY_ptr & KEY0_MASK) // if KEY0 is pressed
    {

      cache_size = 64;

      while ( * KEY_ptr & KEY0_MASK); // wait for KEY0 to be released
      break;

    }

    if ( * KEY_ptr & KEY1_MASK) // if KEY1 is pressed
    {

      cache_size = 128;

      while ( * KEY_ptr & KEY1_MASK); // wait for KEY1 to be released
      break;

    }

    if ( * KEY_ptr & KEY2_MASK) // if KEY2 is pressed
    {

      cache_size = 256;

      while ( * KEY_ptr & KEY2_MASK); // wait for KEY2 to be released
      break;

    }

    if ( * KEY_ptr & KEY3_MASK) // if KEY3 is pressed
    {

      cache_size = 512;

      while ( * KEY_ptr & KEY3_MASK); // wait for KEY3 to be released
      break;

    }

  }

  clear_screen();

  l = 0;

  for (l = 140; l <= 175; l++) {

    write_char(l, 10, 0);

    write_char(l, 23, 0);

    write_char(l, 35, 0);

    write_char(l, 45, 0);

  }

  print_rect_boundary(160, 40, 250, 40, 0xF800);

  char * hw1 = "Press KEY 0 FOR Block Size - 8B";

  x = 140;

  while ( * hw1) {

    write_char(x, 10, * hw1);

    x++;

    hw1++;

  }

  print_rect_boundary(160, 90, 250, 40, 0xF800);

  hw1 = "Press KEY 1 FOR Block Size - 16B";

  x = 140;

  while ( * hw1) {

    write_char(x, 23, * hw1);

    x++;

    hw1++;

  }

  print_rect_boundary(160, 140, 250, 40, 0xF800);

  hw1 = "Press KEY 2 FOR Block Size - 32B";

  x = 140;

  while ( * hw1) {

    write_char(x, 35, * hw1);

    x++;

    hw1++;

  }

  print_rect_boundary(160, 190, 250, 40, 0xF800);

  hw1 = "Press KEY 3 FOR Block Size - 64B";

  x = 140;

  while ( * hw1) {

    write_char(x, 45, * hw1);

    x++;

    hw1++;

  }

  while (1)

  {

    if ( * KEY_ptr & KEY0_MASK) // if KEY0 is pressed
    {

      block_size = 8;

      while ( * KEY_ptr & KEY0_MASK); // wait for KEY0 to be released
      break;

    }

    if ( * KEY_ptr & KEY1_MASK) // if KEY1 is pressed
    {

      block_size = 16;

      while ( * KEY_ptr & KEY1_MASK); // wait for KEY1 to be released
      break;

    }

    if ( * KEY_ptr & KEY2_MASK) // if KEY2 is pressed
    {

      block_size = 32;

      while ( * KEY_ptr & KEY2_MASK); // wait for KEY2 to be released
      break;

    }

    if ( * KEY_ptr & KEY3_MASK) // if KEY3 is pressed
    {

      block_size = 64;

      while ( * KEY_ptr & KEY3_MASK); // wait for KEY3 to be released
      break;

    }

  }

  clear_screen();

  l = 0;

  for (l = 140; l <= 175; l++) {

    write_char(l, 10, 0);

    write_char(l, 23, 0);

    write_char(l, 35, 0);

    write_char(l, 45, 0);

  }

  int nums = 0;

  char * m = "Enter Number Of Block Accesses to main memory ->";

  printf("%s", m);

  scanf("%d", & nums);

  char * message = "Enter the access sequence of memory words ->";

  printf("%s", message);

  int Block_Access[100] = {
    0
  };

  int k = 0;

  while (k < nums) {

    scanf("%d", & Block_Access[k]);

    k++;

  }
     int n;
	n=0;
  while(cache_size != 0){
	  n++;
	  cache_size = cache_size - block_size;
  }
  printf("%d",n);
  int temp = n;

  int y = 160;

  int width = 10;

  if (n <= 10)

  {

    while (temp--)

    {

      print_rect_boundary(160, y, 200, width, 0xFFFFFF);

      int z;

      z = n - temp - 1;

      write_int(10, y / 4, z);

      y = y - 15;

    }
  } else if (n > 10 && n <= 20)

  {

    y = 200;

    width = 5;

    while (temp--)

    {

      print_rect_boundary(160, y, 200, width, 0xFFFFFF);

      int z;

      z = n - temp - 1;

      write_int(10, y / 4, z);

      y = y - 10;

    }
  } else if (n > 20 && n <= 32)

  {

    y = 220;

    width = 5;

    while (temp--)

    {

      print_rect_boundary(160, y, 200, width, 0xFFFFFF);

      int z;

      z = n - temp - 1;

      write_int(10, y / 4, z);

      y = y - 7;

    }
  }

  int visited[100] = {
    0
  };

  int i = -1;

  char * msg = "Access for Block Number - ";

  x = 20;

  while ( * msg) {

    write_char(x, 3, * msg);

    x++;

    msg++;

  }

  while (i < 100)

  {

    while (1)

    {

      if ( * KEY_ptr & KEY0_MASK) // if KEY0 is pressed
      {

        i++;

        while ( * KEY_ptr & KEY0_MASK); // wait for KEY0 to be released
        break;

      }

    }

    if (Block_Access[i] != 0)

    {

      int x = Block_Access[i];
      printf("%d",x);
      write_char(45, 3, 0);

      write_char(46, 3, 0);

      write_int(45, 3, x);

      // int p =  x % 16;
      int xx, p;

      xx = n;

      for (xx = n; xx < n + 1; xx++)

        p = x % xx;

      // printf("\nmodulo = %d\n", x%xx);
      // printf("%d",p);
      //change_colour(p,n,Block_Access,visited,x);
      y = 160;

      if (n <= 10)

      {

        if (visited[p] == 0)

        {

          draw_rectangle(160, y - 15 * (p), 200, 10, 0xF800);

          write_char(40, (y - 15 * p) / 4, 0);

          write_char(41, (y - 15 * p) / 4, 0);

          write_int(40, (y - 15 * p) / 4, x);

          visited[p] = x;

        } else if (visited[p] == x) {

          draw_rectangle(160, y - 15 * (p), 200, 10, 0x07E0);

          write_char(40, (y - 15 * p) / 4, 0);

          write_char(41, (y - 15 * p) / 4, 0);

          write_int(40, (y - 15 * p) / 4, x);

          hits++;

        } else

        {
          // miss

          draw_rectangle(160, y - 15 * (p), 200, 10, 0xF800);

          write_char(40, (y - 15 * p) / 4, 0);

          write_char(41, (y - 15 * p) / 4, 0);

          write_int(40, (y - 15 * p) / 4, x);
			visited[p] = x;
          // red 
        }

      } else if (n > 10 && n <= 20)

      {

        y = 200;

        if (visited[p] == 0)

        {

          draw_rectangle(160, y - 10 * (p), 200, 5, 0xF800);

          write_char(40, (y - 10 * p) / 4, 0);

          write_char(41, (y - 10 * p) / 4, 0);

          write_int(40, (y - 10 * p) / 4, x);

          visited[p] = x;

        } else if (visited[p] == x) {

          draw_rectangle(160, y - 10 * (p), 200, 5, 0x07E0);

          write_char(40, (y - 10 * p) / 4, 0);

          write_char(41, (y - 10 * p) / 4, 0);

          write_int(40, (y - 10 * p) / 4, x);

          hits++;

        } else

        {

          draw_rectangle(160, y - 10 * (p), 200, 5, 0xF800);

          write_char(40, (y - 10 * p) / 4, 0);

          write_char(41, (y - 10 * p) / 4, 0);

          write_int(40, (y - 10 * p) / 4, x);
			visited[p] = x;
          // red 
        }

      } else if (n > 20 && n <= 32)

      {

        y = 220;

        if (visited[p] == 0)

        {

          draw_rectangle(160, y - 7 * (p), 200, 5, 0xF800);

          write_char(40, (y - 7 * p) / 4, 0);

          write_char(41, (y - 7 * p) / 4, 0);

          write_int(40, (y - 7 * p) / 4, x);

          visited[p] = x;

        } else if (visited[p] == x) {

          draw_rectangle(160, y - 7 * (p), 200, 5, 0x07E0);

          write_char(40, (y - 7 * p) / 4, 0);

          write_char(41, (y - 7 * p) / 4, 0);

          write_int(40, (y - 7 * p) / 4, x);

          hits++;

        } else

        {

          draw_rectangle(160, y - 7 * (p), 200, 5, 0xF800);

          write_char(40, (y - 7 * p) / 4, 0);

          write_char(41, (y - 7 * p) / 4, 0);

          write_int(40, (y - 7 * p) / 4, x);
			visited[p] = x;

        }

      }

    } else {

      break;

    }

  }

  clear_screen();

  int q;

  for (q = 20; q <= 46; q++) {

    write_char(q, 3, 0);

  }
  int pp;
  for(pp=0;pp<60;pp++)
  {
    write_char(40, pp, 0);
    write_char(41, pp, 0);
  }
  

  int temp1 = n;

  y = 160;

  if (n <= 10) {

    while (temp1--)

    {

      write_char(10, y / 4, 0);

      y = y - 15;

    }
  } else if (n > 10 && n <= 20)

  {

    y = 200;

    while (temp1--)

    {

      write_char(10, y / 4, 0);

      write_char(11, y / 4, 0);

      y = y - 10;

    }
  } else if (n > 20 && n <= 32)

  {

    y = 220;

    while (temp1--)

    {

      write_char(10, y / 4, 0);

      write_char(11, y / 4 , 0);

      y = y - 7;

    }
  }

  hits = hits * 100;

  int hit_ratio = hits / nums;

  hits = hits / 100;

  print_rect_boundary(160, 40, 250, 40, 0xF800);

  char * hw4 = "Number Of Hits - ";

  x = 140;

  while ( * hw4) {

    write_char(x, 10, * hw4);

    x++;

    hw4++;

  }

  write_int(x + 1, 10, hits);

  print_rect_boundary(160, 90, 250, 40, 0xF800);

  hw4 = "Total Number Of Block Accesses - ";

  x = 140;

  while ( * hw4) {

    write_char(x, 23, * hw4);

    x++;

    hw4++;

  }

  write_int(x + 1, 23, nums);

  print_rect_boundary(160, 140, 250, 40, 0xF800);

  hw4 = "Hit Ratio - ";

  x = 140;

  while ( * hw4) {

    write_char(x, 35, * hw4);

    x++;

    hw4++;

  }

  write_int(x + 1, 35, hit_ratio);
    while (1)

    {

      if ( * KEY_ptr & KEY0_MASK) // if KEY0 is pressed
      {
        while ( * KEY_ptr & KEY0_MASK); // wait for KEY0 to be released
        break;

      }

    }
    l = 0;

  for (l = 140; l <= 175; l++) {

    write_char(l, 10, 0);

    write_char(l, 23, 0);

    write_char(l, 35, 0);

  }
clear_screen();

  return 0;

}