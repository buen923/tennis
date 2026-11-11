
#include <Arduboy2.h>
#include <math.h>

Arduboy2 arduboy;

// 3D座標
struct Point3D {
  float x;
  float y;
  float z;
};

// 2D座標
struct Point2D {
  int x;
  int y;
};


// // 立方体の頂点
// const Point3D vertices[4] = {
//   { 6,0,-2},
//   { 6,0,2},
//   {-6,0,2},
//   {-6,0,-2},
// };
struct Cube {
  int x,y,z,w,h,d;
  Point3D vertices[8];
  


uint8_t edges[12][2] = {
  {0,1}, {1,2}, {2,3}, {3,0},
  {4,5}, {5,6}, {6,7}, {7,4},
  {0,4}, {1,5}, {2,6}, {3,7}
};

Cube(int _x,int _y,int _z,int _w,int _h,int _d) {
  x=_x;y=_y;z=_z;w=_w;h=_h;d=_d;
  vertices[0] = {_x-_w, _y-_h, _z+_d};
  vertices[1] = {_x-_w, _y+_h, _z+_d};
  vertices[2] = {_x+_w, _y+_h, _z+_d};
  vertices[3] = {_x+_w, _y-_h, _z+_d};
  vertices[4] = {_x-_w, _y-_h, _z-_d};
  vertices[5] = {_x-_w, _y+_h, _z-_d};
  vertices[6] = {_x+_w, _y+_h, _z-_d};
  vertices[7] = {_x+_w, _y-_h, _z-_d};
};



};
Cube court={1,3,2,3,1,1};

// 回転角度
float angleX = 0;
float angleY = 0;

// 回転速度
const float ROT_SPEED = 0.05;

// 3D座標を2D座標に変換
Point2D project(Point3D p) {

  const float scale = 55.0;
  const float distance = 8.0;

  Point2D result;

  float depth = p.z + distance;
  Serial.println(depth);
  result.x = 64 + scale * p.x / depth;
  result.y = 32 - scale * p.y / depth;

  return result;
}

// X軸・Y軸回転
Point3D rotatePoint(Point3D p) {

  Point3D result;

  // Y軸回転（左右）
  float x1 = p.x * cos(angleY)
           + p.z * sin(angleY);

  float z1 = -p.x * sin(angleY)
           + p.z * cos(angleY);

  // X軸回転（上下）
  float y2 = p.y * cos(angleX)
           - z1 * sin(angleX);

  float z2 = p.y * sin(angleX)
           + z1 * cos(angleX);

  result.x = x1;
  result.y = y2;
  result.z = z2;

  return result;
}

// 立方体を描画
void drawCube() {

  Point2D screen[8];

  // 頂点を回転して画面座標に変換
  for (int i = 0; i < 8;i++) {

    Point3D rotated = rotatePoint(court.vertices[i]);

    screen[i] = project(rotated);
  }

  // 頂点同士を線で結ぶ
  for (int i = 0; i < 12; i++) {

    uint8_t a = court.edges[i][0];
    uint8_t b = court.edges[i][1];

    arduboy.drawLine(
      screen[a].x,
      screen[a].y,
      screen[b].x,
      screen[b].y,
      WHITE
    );
  }
}

void setup() {

  arduboy.begin();
  Serial.begin(9600);
  arduboy.setFrameRate(30);
}

void loop() {

  if (!arduboy.nextFrame()) return;

  arduboy.pollButtons();

  arduboy.clear();

  // 左右回転Simulator

  if (arduboy.pressed(LEFT_BUTTON)) {
    angleY -= ROT_SPEED;
  }

  if (arduboy.pressed(RIGHT_BUTTON)) {
    angleY += ROT_SPEED;
  }

  // 上下回転
  if (arduboy.pressed(UP_BUTTON)) {
    angleX += ROT_SPEED;
  }

  if (arduboy.pressed(DOWN_BUTTON)) {
    angleX -= ROT_SPEED;
  }

  // 立方体描画
  drawCube();
  arduboy.display();
}
