// push button base address
volatile int *push_button = 0xff200050;

/* set a single pixel on the screen at x,y
 * x in [0,319], y in [0,239], and colour in [0,65535]
 */

// volatile int *push_button=0xff200050;

void write_pixel(int x, int y, short int colour)
{
  volatile short *vga_addr = (volatile short *)(0xc8000000 + (y << 10) + (x << 1));
  *vga_addr = colour;
}

/* use write_pixel to set entire screen to black (does not clear the character buffer) */
void clear_screen()
{
  int x, y;
  for (x = 0; x < 320; x++)
  {
    for (y = 0; y < 240; y++)
    {
      write_pixel(x, y, 0);
    }
  }
}

void clear_screen2()
{
  int x, y;
  for (x = 0; x < 320; x++)
  {
    for (y = 30; y < 200; y++)
    {
      write_pixel(x, y, 0);
    }
  }
}

/* write a single character to the character buffer at x,y
 * x in [0,79], y in [0,59]
 */
void write_char(int x, int y, char c)
{
  // VGA character buffer
  volatile char *character_buffer = (char *)(0xc9000000 + (y << 7) + x);
  *character_buffer = c;
}

void delay()
{

  volatile int *pixel_ctrl_ptr = (int *)0xff203020;
  int status;
  *pixel_ctrl_ptr = 1;
  status = *(pixel_ctrl_ptr + 3);

  while ((status & 0x01) != 0)
  {

    status = *(pixel_ctrl_ptr + 3);
  }
}

void horizontal_line_y(int y, short int c)
{
  int x;
  for (x = 0; x < 320; x++)
  { // Draw a straight line in red across the screen centre
    write_pixel(x, y, 0xf800);
  }
}

void player(int x, int y, short int c)
{
  for (int i = x; i < x + 5; i++)
  {
    for (int j = y; j < y + 5; j++)
    {
      write_pixel(i, j, c);
    }
  }
}

int playerX = 150;
int playerY = 10;

void move_player(int move, int x, int y)
{
  // move up
  if (move == 1)
  {
    delay();
    delay();
    player(playerX, playerY, 0);
    if (y > 20)
      playerY -= 20;
  }

  // move down
  if (move == 2)
  {
    delay();
    delay();
    player(playerX, playerY, 0);
    if (y < 219)
      playerY += 20;
  }

  // move left
  if (move == 4)
  {
    delay();
    delay();
    player(playerX, playerY, 0);
    if (x > 20)
      playerX -= 20;
  }

  // move right
  if (move == 8)
  {
    delay();
    delay();
    player(playerX, playerY, 0);
    if (x < 299)
      playerX += 20;
  }
}

void draw_rectangle(int x, short int c)
{
  for (int i = x; i < x + 21; i++)
  {
    for (int j = 40; j < 61; j++)
    {
      write_pixel(i, j, c);
    }
  }
}
void draw_rectangle2(int x, short int c)
{
  for (int i = x; i < x + 21; i++)
  {
    for (int j = 80; j < 101; j++)
    {
      write_pixel(i, j, c);
    }
  }
}
void draw_rectangle3(int x, short int c)
{
  for (int i = x; i < x + 31; i++)
  {
    for (int j = 110; j < 131; j++)
    {
      write_pixel(i, j, c);
    }
  }
}

void draw_rectangle4(int x, short int c)
{
  for (int i = x; i < x + 11; i++)
  {
    for (int j = 150; j < 181; j++)
    {
      write_pixel(i, j, c);
    }
  }
}

int main()
{

  int start_game = 1;

  clear_screen();

  int increment = 5;
  int increment2 = 110;
  int increment3 = 50;
  int increment4 = 30;

  int x;
  // Welcoming the user !
  char *hw2 = "Play the game";
  x = 10;
  while (*hw2)
  {
    write_char(x, 3, *hw2);
    x++;
    hw2++;
  }

  int collision = 0;

  while (1)
  {
    int value = *push_button;
    if (start_game == 0)
    {
      if (*push_button != 8)
      {
        continue;
        // No Restarting
      }
      else
      {
        clear_screen2();
        clear_screen();
        int x, y;
        for (x = 0; x < 80; x++)
        {
          for (y = 0; y < 60; y++)
          {
            write_char(x, y, ' ');
          }
        }
         playerX = 150;
         playerY = 10;
        collision = 0;
      }
    }
    while (1)
    {
      int value = *push_button;
      player(playerX, playerY, 2000);
      horizontal_line_y(210, 600);
      draw_rectangle(increment, 600);
      draw_rectangle2(increment2, 600);
      draw_rectangle3(increment3, 600);
      draw_rectangle4(increment4, 600);
      delay();
      draw_rectangle(increment, 0);
      draw_rectangle2(increment2, 0);
      draw_rectangle3(increment3, 0);
      draw_rectangle4(increment4, 0);
      increment += 10;
      increment2 += 10;
      increment3 += 10;
      increment4 += 10;

      if (increment % 280 == 0)
        increment = 5;
      if (increment2 % 280 == 0)
        increment2 = 110;
      if (increment3 % 270 == 0)
        increment3 = 50;
      if (increment4 % 290 == 0)
        increment4 = 30;
      if (value != 0)
        move_player(value, playerX, playerY);

      // check collisioon
      if (playerY >= 40 && playerY < 61)
      {
        if (playerX >= increment && playerX < (increment + 21))
        {
          collision = 2;
          start_game = 0;
          break;
        }
      }

      if (playerY >= 80 && playerY < 101)
      {
        if (playerX >= increment2 && playerX < (increment2 + 21))
        {
          collision = 2;
          start_game = 0;
          break;
        }
      }

      if (playerY >= 110 && playerY < 131)
      {
        if (playerX >= increment3 && playerX < (increment3 + 31))
        {
          collision = 2;
          start_game = 0;
          break;
        }
      }

      if (playerY >= 150 && playerY < 181)
      {
        if (playerX >= increment4 && playerX < (increment4 + 11))
        {
          collision = 2;
          start_game = 0;
          break;
        }
      }

      if (playerY > 210)
      {
        start_game = 0;
        break;
      }
    }

    if (collision != 2)
    {

      for (int u = 0; u < 320; u++)
      {
        for (int i = 0; i < 230; i++)
        {
          write_pixel(u, i, 3000);
        }
      }

      int m;
      char *hw1 = "You win the game";
      m = 10;
      while (*hw1)
      {
        write_char(m, 3, *hw1);
        m++;
        hw1++;
      }
    }
    else
    {

      for (int u = 0; u < 320; u++)
      {
        for (int i = 0; i < 230; i++)
        {
          write_pixel(u, i, 1000);
        }
      }
      int m;
      char *hw1 = "You lose the game";
      m = 10;
      while (*hw1)
      {
        write_char(m, 3, *hw1);
        m++;
        hw1++;
      }
    }
  }

  return 0;
}