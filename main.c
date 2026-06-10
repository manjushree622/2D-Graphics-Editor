#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

// --- Constants ---
#define WIDTH 80        // Width of the graphics canvas
#define HEIGHT 25       // Height of the graphics canvas
#define MAX_OBJECTS 100 // Maximum number of objects that can be stored

// --- Enumerations ---
typedef enum {
  SHAPE_RECTANGLE = 1,
  SHAPE_LINE,
  SHAPE_TRIANGLE,
  SHAPE_CIRCLE
} ShapeType;

// --- Shape Details Structs ---
typedef struct {
  int x;      // Top-left x-coordinate
  int y;      // Top-left y-coordinate
  int width;  // Width of the rectangle
  int height; // Height of the rectangle
} RectData;

typedef struct {
  int x1, y1; // Starting point coordinates
  int x2, y2; // Ending point coordinates
} LineData;

typedef struct {
  int x1, y1; // Vertex 1 coordinates
  int x2, y2; // Vertex 2 coordinates
  int x3, y3; // Vertex 3 coordinates
} TriangleData;

typedef struct {
  int cx, cy; // Center coordinates (x, y)
  int r;      // Radius of the circle
} CircleData;

// --- Unified Shape Registry Struct ---
typedef struct {
  int id;         // Unique identifier of the object
  ShapeType type; // Shape identifier
  union {
    RectData rect;
    LineData line;
    TriangleData triangle;
    CircleData circle;
  } data;
} ShapeObject;

// --- Global Variables ---
char canvas[HEIGHT][WIDTH];
ShapeObject objects[MAX_OBJECTS];
int object_count = 0;
int next_object_id = 1;

// --- Function Declarations ---
void init_canvas(void);
void set_pixel(int x, int y);
void display_canvas(void);
void draw_line(int x1, int y1, int x2, int y2);
void draw_rectangle(int x, int y, int w, int h);
void draw_triangle(int x1, int y1, int x2, int y2, int x3, int y3);
void draw_circle(int cx, int cy, int r);
void redraw_canvas(void);

void add_rectangle(void);
void add_line(void);
void add_triangle(void);
void add_circle(void);
void delete_object(void);
void modify_object(void);
void clear_all_objects(void);

// Safe input helper declarations
int read_int(const char *prompt, int min, int max);
void print_menu(void);

// --- MAIN FUNCTION ---
int main(void) {
  init_canvas();
  int choice = 0;

  printf("====================================================\n");
  printf("        WELCOME TO THE 2D GRAPHICS EDITOR IN C     \n");
  printf("====================================================\n");
  printf("Canvas Resolution: %d x %d characters.\n", WIDTH, HEIGHT);
  printf("Bresenham's Rendering algorithms initialized.\n\n");

  while (1) {
    print_menu();
    choice = read_int("Enter your choice (1-9): ", 1, 9);

    switch (choice) {
    case 1:
      display_canvas();
      break;
    case 2:
      add_rectangle();
      break;
    case 3:
      add_line();
      break;
    case 4:
      add_triangle();
      break;
    case 5:
      add_circle();
      break;
    case 6:
      delete_object();
      break;
    case 7:
      modify_object();
      break;
    case 8:
      clear_all_objects();
      break;
    case 9:
      printf("\nExiting the graphics editor. Thank you for using!\n");
      return 0;
    default:
      printf("Unexpected input choice.\n");
      break;
    }
  }
  return 0;
}

// --- Menu UI Print ---
void print_menu(void) {
  printf("\n============ EDITOR CONTROLS ============\n");
  printf("1. Display Canvas\n");
  printf("2. Add Rectangle\n");
  printf("3. Add Line\n");
  printf("4. Add Triangle\n");
  printf("5. Add Circle\n");
  printf("6. Delete Object\n");
  printf("7. Modify Object\n");
  printf("8. Clear Canvas (Delete All)\n");
  printf("9. Exit Editor\n");
  printf("=========================================\n");
}

// --- Safe Input Parsing Helpers ---
int read_int(const char *prompt, int min, int max) {
  char buffer[256];
  long val;
  char *endptr;

  while (1) {
    printf("%s", prompt);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
      printf("Error reading console input. Please try again.\n");
      continue;
    }

    // Parse integer using strtol for error resistance
    val = strtol(buffer, &endptr, 10);

    // If no digits parsed at all
    if (endptr == buffer) {
      printf("Invalid input. Please enter a valid number.\n");
      continue;
    }

    // Trim trailing spaces / newline
    while (*endptr != '\0' && isspace((unsigned char)*endptr)) {
      endptr++;
    }

    // If characters remain after integer, it's invalid format
    if (*endptr != '\0') {
      printf(
          "Invalid input. Please enter ONLY the integer coordinate/option.\n");
      continue;
    }

    // Range check
    if (val < min || val > max) {
      printf(
          "Value out of range. Must be between %d and %d. Please try again.\n",
          min, max);
      continue;
    }

    return (int)val;
  }
}

// --- Canvas Logic Functions ---

/**
 * Initializes the canvas 2D array buffer with background characters.
 */
void init_canvas(void) {
  for (int y = 0; y < HEIGHT; y++) {
    for (int x = 0; x < WIDTH; x++) {
      canvas[y][x] = '_';
    }
  }
}

/**
 * Draws a pixel at coordinates (x, y) if it falls inside the canvas boundaries.
 */
void set_pixel(int x, int y) {
  if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
    canvas[y][x] = '*';
  }
}

/**
 * Displays the current canvas layout complete with column and row axis markings
 * to facilitate exact point insertion.
 */
void display_canvas(void) {
  printf("\n");

  // 1. Column indexes: Tens line
  printf("     ");
  for (int x = 0; x < WIDTH; x++) {
    if (x % 10 == 0) {
      printf("%d", x / 10);
    } else {
      printf(" ");
    }
  }
  printf("\n");

  // 2. Column indexes: Units line
  printf("     ");
  for (int x = 0; x < WIDTH; x++) {
    printf("%d", x % 10);
  }
  printf("\n");

  // 3. Top Canvas frame line
  printf("    +");
  for (int x = 0; x < WIDTH; x++) {
    printf("-");
  }
  printf("+\n");

  // 4. Print Rows with prefix indexes
  for (int y = 0; y < HEIGHT; y++) {
    printf("%2d  |", y);
    for (int x = 0; x < WIDTH; x++) {
      printf("%c", canvas[y][x]);
    }
    printf("|\n");
  }

  // 5. Bottom Canvas frame line
  printf("    +");
  for (int x = 0; x < WIDTH; x++) {
    printf("-");
  }
  printf("+\n");
}

// --- Graphical Draw Algorithms ---

/**
 * Implements Bresenham's Line Algorithm for any slope and orientation.
 */
void draw_line(int x1, int y1, int x2, int y2) {
  int dx = abs(x2 - x1);
  int dy = abs(y2 - y1);
  int sx = (x1 < x2) ? 1 : -1;
  int sy = (y1 < y2) ? 1 : -1;
  int err = dx - dy;

  while (1) {
    set_pixel(x1, y1);
    if (x1 == x2 && y1 == y2) {
      break;
    }
    int e2 = 2 * err;
    if (e2 > -dy) {
      err -= dy;
      x1 += sx;
    }
    if (e2 < dx) {
      err += dx;
      y1 += sy;
    }
  }
}

/**
 * Renders an outline rectangle.
 */
void draw_rectangle(int x, int y, int w, int h) {
  // Top border
  for (int i = 0; i < w; i++) {
    set_pixel(x + i, y);
  }
  // Bottom border
  for (int i = 0; i < w; i++) {
    set_pixel(x + i, y + h - 1);
  }
  // Left border
  for (int i = 0; i < h; i++) {
    set_pixel(x, y + i);
  }
  // Right border
  for (int i = 0; i < h; i++) {
    set_pixel(x + w - 1, y + i);
  }
}

/**
 * Renders a triangle by connecting three coordinate points with lines.
 */
void draw_triangle(int x1, int y1, int x2, int y2, int x3, int y3) {
  draw_line(x1, y1, x2, y2);
  draw_line(x2, y2, x3, y3);
  draw_line(x3, y3, x1, y1);
}

/**
 * Midpoint / Bresenham's Circle Drawing Algorithm.
 */
void draw_circle(int cx, int cy, int r) {
  if (r < 0)
    return;
  int x = 0;
  int y = r;
  int d = 3 - 2 * r;
  while (y >= x) {
    set_pixel(cx + x, cy + y);
    set_pixel(cx - x, cy + y);
    set_pixel(cx + x, cy - y);
    set_pixel(cx - x, cy - y);
    set_pixel(cx + y, cy + x);
    set_pixel(cx - y, cy + x);
    set_pixel(cx + y, cy - x);
    set_pixel(cx - y, cy - x);
    if (d < 0) {
      d = d + 4 * x + 6;
    } else {
      d = d + 4 * (x - y) + 10;
      y--;
    }
    x++;
  }
}

/**
 * Clears the canvas buffer and replots all active shapes stored in the object
 * list.
 */
void redraw_canvas(void) {
  init_canvas();
  for (int i = 0; i < object_count; i++) {
    ShapeObject obj = objects[i];
    switch (obj.type) {
    case SHAPE_RECTANGLE:
      draw_rectangle(obj.data.rect.x, obj.data.rect.y, obj.data.rect.width,
                     obj.data.rect.height);
      break;
    case SHAPE_LINE:
      draw_line(obj.data.line.x1, obj.data.line.y1, obj.data.line.x2,
                obj.data.line.y2);
      break;
    case SHAPE_TRIANGLE:
      draw_triangle(obj.data.triangle.x1, obj.data.triangle.y1,
                    obj.data.triangle.x2, obj.data.triangle.y2,
                    obj.data.triangle.x3, obj.data.triangle.y3);
      break;
    case SHAPE_CIRCLE:
      draw_circle(obj.data.circle.cx, obj.data.circle.cy, obj.data.circle.r);
      break;
    }
  }
}

// --- CRUD Control Operations ---

/**
 * Solicits Rectangle dimensions and appends it to the registry.
 */
void add_rectangle(void) {
  if (object_count >= MAX_OBJECTS) {
    printf("Error: Canvas objects memory limit reached!\n");
    return;
  }
  printf("\n--- Adding Rectangle ---\n");
  int x = read_int("Enter Top-Left X coordinate (0-79): ", 0, WIDTH - 1);
  int y = read_int("Enter Top-Left Y coordinate (0-24): ", 0, HEIGHT - 1);
  int w = read_int("Enter Width (1-80): ", 1, WIDTH);
  int h = read_int("Enter Height (1-25): ", 1, HEIGHT);

  ShapeObject new_rect;
  new_rect.id = next_object_id++;
  new_rect.type = SHAPE_RECTANGLE;
  new_rect.data.rect.x = x;
  new_rect.data.rect.y = y;
  new_rect.data.rect.width = w;
  new_rect.data.rect.height = h;

  objects[object_count++] = new_rect;
  redraw_canvas();
  printf("Rectangle added successfully! (ID: %d)\n", new_rect.id);
}

/**
 * Solicits Line dimensions and appends it to the registry.
 */
void add_line(void) {
  if (object_count >= MAX_OBJECTS) {
    printf("Error: Canvas objects memory limit reached!\n");
    return;
  }
  printf("\n--- Adding Line ---\n");
  int x1 = read_int("Enter Start Point X1 (0-79): ", 0, WIDTH - 1);
  int y1 = read_int("Enter Start Point Y1 (0-24): ", 0, HEIGHT - 1);
  int x2 = read_int("Enter End Point X2 (0-79): ", 0, WIDTH - 1);
  int y2 = read_int("Enter End Point Y2 (0-24): ", 0, HEIGHT - 1);

  ShapeObject new_line;
  new_line.id = next_object_id++;
  new_line.type = SHAPE_LINE;
  new_line.data.line.x1 = x1;
  new_line.data.line.y1 = y1;
  new_line.data.line.x2 = x2;
  new_line.data.line.y2 = y2;

  objects[object_count++] = new_line;
  redraw_canvas();
  printf("Line added successfully! (ID: %d)\n", new_line.id);
}

/**
 * Solicits Triangle dimensions and appends it to the registry.
 */
void add_triangle(void) {
  if (object_count >= MAX_OBJECTS) {
    printf("Error: Canvas objects memory limit reached!\n");
    return;
  }
  printf("\n--- Adding Triangle ---\n");
  int x1 = read_int("Enter Vertex 1 X1 (0-79): ", 0, WIDTH - 1);
  int y1 = read_int("Enter Vertex 1 Y1 (0-24): ", 0, HEIGHT - 1);
  int x2 = read_int("Enter Vertex 2 X2 (0-79): ", 0, WIDTH - 1);
  int y2 = read_int("Enter Vertex 2 Y2 (0-24): ", 0, HEIGHT - 1);
  int x3 = read_int("Enter Vertex 3 X3 (0-79): ", 0, WIDTH - 1);
  int y3 = read_int("Enter Vertex 3 Y3 (0-24): ", 0, HEIGHT - 1);

  ShapeObject new_triangle;
  new_triangle.id = next_object_id++;
  new_triangle.type = SHAPE_TRIANGLE;
  new_triangle.data.triangle.x1 = x1;
  new_triangle.data.triangle.y1 = y1;
  new_triangle.data.triangle.x2 = x2;
  new_triangle.data.triangle.y2 = y2;
  new_triangle.data.triangle.x3 = x3;
  new_triangle.data.triangle.y3 = y3;

  objects[object_count++] = new_triangle;
  redraw_canvas();
  printf("Triangle added successfully! (ID: %d)\n", new_triangle.id);
}

/**
 * Solicits Circle dimensions and appends it to the registry.
 */
void add_circle(void) {
  if (object_count >= MAX_OBJECTS) {
    printf("Error: Canvas objects memory limit reached!\n");
    return;
  }
  printf("\n--- Adding Circle ---\n");
  int cx = read_int("Enter Center X (0-79): ", 0, WIDTH - 1);
  int cy = read_int("Enter Center Y (0-24): ", 0, HEIGHT - 1);
  int r = read_int("Enter Radius (0-40): ", 0, WIDTH / 2);

  ShapeObject new_circle;
  new_circle.id = next_object_id++;
  new_circle.type = SHAPE_CIRCLE;
  new_circle.data.circle.cx = cx;
  new_circle.data.circle.cy = cy;
  new_circle.data.circle.r = r;

  objects[object_count++] = new_circle;
  redraw_canvas();
  printf("Circle added successfully! (ID: %d)\n", new_circle.id);
}

/**
 * Helper to display current shapes stored in the registry.
 */
void list_objects(void) {
  printf("\n--- Active Canvas Objects ---\n");
  for (int i = 0; i < object_count; i++) {
    ShapeObject obj = objects[i];
    printf("[%d] ID: %d | ", i + 1, obj.id);
    switch (obj.type) {
    case SHAPE_RECTANGLE:
      printf("Rectangle (Top-Left: %d,%d, Width: %d, Height: %d)\n",
             obj.data.rect.x, obj.data.rect.y, obj.data.rect.width,
             obj.data.rect.height);
      break;
    case SHAPE_LINE:
      printf("Line (Start: %d,%d -> End: %d,%d)\n", obj.data.line.x1,
             obj.data.line.y1, obj.data.line.x2, obj.data.line.y2);
      break;
    case SHAPE_TRIANGLE:
      printf("Triangle (P1: %d,%d, P2: %d,%d, P3: %d,%d)\n",
             obj.data.triangle.x1, obj.data.triangle.y1, obj.data.triangle.x2,
             obj.data.triangle.y2, obj.data.triangle.x3, obj.data.triangle.y3);
      break;
    case SHAPE_CIRCLE:
      printf("Circle (Center: %d,%d, Radius: %d)\n", obj.data.circle.cx,
             obj.data.circle.cy, obj.data.circle.r);
      break;
    }
  }
  printf("-------------------------------\n");
}

/**
 * Deletes a chosen shape by shifting remaining elements and triggers canvas
 * redraw.
 */
void delete_object(void) {
  if (object_count == 0) {
    printf("\nNo objects to delete.\n");
    return;
  }

  list_objects();
  int choice = read_int("Enter the index of the object to delete (1-N): ", 1,
                        object_count);
  int target_idx = choice - 1;

  int deleted_id = objects[target_idx].id;

  // Shift remaining items left in array
  for (int i = target_idx; i < object_count - 1; i++) {
    objects[i] = objects[i + 1];
  }
  object_count--;

  redraw_canvas();
  printf("Object ID %d deleted successfully.\n", deleted_id);
}

/**
 * Modifies shape parameters at a particular index and updates canvas.
 */
void modify_object(void) {
  if (object_count == 0) {
    printf("\nNo objects to modify.\n");
    return;
  }

  list_objects();
  int choice = read_int("Enter the index of the object to modify (1-N): ", 1,
                        object_count);
  int target_idx = choice - 1;

  ShapeObject *obj = &objects[target_idx];
  printf("\nModifying Object ID %d:\n", obj->id);

  switch (obj->type) {
  case SHAPE_RECTANGLE: {
    int x = read_int("Enter new Top-Left X (0-79): ", 0, WIDTH - 1);
    int y = read_int("Enter new Top-Left Y (0-24): ", 0, HEIGHT - 1);
    int w = read_int("Enter new Width (1-80): ", 1, WIDTH);
    int h = read_int("Enter new Height (1-25): ", 1, HEIGHT);
    obj->data.rect.x = x;
    obj->data.rect.y = y;
    obj->data.rect.width = w;
    obj->data.rect.height = h;
    break;
  }
  case SHAPE_LINE: {
    int x1 = read_int("Enter new Start X1 (0-79): ", 0, WIDTH - 1);
    int y1 = read_int("Enter new Start Y1 (0-24): ", 0, HEIGHT - 1);
    int x2 = read_int("Enter new End X2 (0-79): ", 0, WIDTH - 1);
    int y2 = read_int("Enter new End Y2 (0-24): ", 0, HEIGHT - 1);
    obj->data.line.x1 = x1;
    obj->data.line.y1 = y1;
    obj->data.line.x2 = x2;
    obj->data.line.y2 = y2;
    break;
  }
  case SHAPE_TRIANGLE: {
    int x1 = read_int("Enter new Vertex 1 X1 (0-79): ", 0, WIDTH - 1);
    int y1 = read_int("Enter new Vertex 1 Y1 (0-24): ", 0, HEIGHT - 1);
    int x2 = read_int("Enter new Vertex 2 X2 (0-79): ", 0, WIDTH - 1);
    int y2 = read_int("Enter new Vertex 2 Y2 (0-24): ", 0, HEIGHT - 1);
    int x3 = read_int("Enter new Vertex 3 X3 (0-79): ", 0, WIDTH - 1);
    int y3 = read_int("Enter new Vertex 3 Y3 (0-24): ", 0, HEIGHT - 1);
    obj->data.triangle.x1 = x1;
    obj->data.triangle.y1 = y1;
    obj->data.triangle.x2 = x2;
    obj->data.triangle.y2 = y2;
    obj->data.triangle.x3 = x3;
    obj->data.triangle.y3 = y3;
    break;
  }
  case SHAPE_CIRCLE: {
    int cx = read_int("Enter new Center CX (0-79): ", 0, WIDTH - 1);
    int cy = read_int("Enter new Center CY (0-24): ", 0, HEIGHT - 1);
    int r = read_int("Enter new Radius (0-40): ", 0, WIDTH / 2);
    obj->data.circle.cx = cx;
    obj->data.circle.cy = cy;
    obj->data.circle.r = r;
    break;
  }
  }

  redraw_canvas();
  printf("Object ID %d updated successfully.\n", obj->id);
}

/**
 * Resets the shape registries and clears the drawing grid.
 */
void clear_all_objects(void) {
  object_count = 0;
  next_object_id = 1;
  init_canvas();
  printf("\nCanvas cleared successfully. All object records wiped.\n");
}