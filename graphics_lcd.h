void init_lcd();

void set_cursor_home();

void set_cursor_x(uint8_t x);
void set_cursor_y(uint8_t y);

void set_text(char* text, uint8_t len, uint8_t x, uint8_t y);
void set_int(int num, uint8_t x, uint8_t y);

void clear_screen();
