
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
  float angleX,angleY=0;
    float distance = 100.0;
    bool Mode=0;
bool forward;

struct Cube {
  float x,y,z,w,h,d;
  float scale = 100.0;
  Point3D vertices[8];
  


uint8_t edges[12][2] = {
  {0,1}, {1,2}, {2,3}, {3,0},
  {4,5}, {5,6}, {6,7}, {7,4},
  {0,4}, {1,5}, {2,6}, {3,7}
};


  


Cube() {
  x=0;y=0;z=0;w=0;h=0;d=0;
}
  
Cube(int x,int y,int z,float w,float h,float d) {
  
  this->x = x;
  this->y = y;
  this->z = z;
  this->w = w;
  this->h = h;
  this->d = d;


  





};

 Point3D rotateXY (Point3D p) {
  Point3D result;


  // Y軸回転（左右）
  float x1 = p.x * cos(angleY) + p.z * sin(angleY);

  float z1 = -p.x * sin(angleY) + p.z * cos(angleY);

  // X軸回転（上下）
  float y2 = p.y * cos(angleX) - z1 * sin(angleX);

  float z2 = p.y * sin(angleX) + z1 * cos(angleX);

  result.x = x1;
  result.y = y2;
  result.z = z2;

  return result;

  } 

void translate(Point3D d) {
  x+=d.x;
  y+=d.y;
  z+=d.z;
}

// 立方体を描画
void draw() {
  vertices[0] = {x-w, y-h, z+d};
  vertices[1] = {x-w, y+h, z+d};
  vertices[2] = {x+w, y+h, z+d};
  vertices[3] = {x+w, y-h, z+d};
  vertices[4] = {x-w, y-h, z-d};
  vertices[5] = {x-w, y+h, z-d};
  vertices[6] = {x+w, y+h, z-d};
  vertices[7] = {x+w, y-h, z-d};
  Point2D screen[8];


  // 頂点を回転して画面座標に変換
  for (int i = 0; i < 8;i++) {

  Point3D rotated = rotateXY(vertices[i]);
  float depth = rotated.z + distance;
  if(depth<1) {depth=1;}
  screen[i].x = 64 + scale * rotated.x / depth;
  screen[i].y = 32 - scale * rotated.y / depth;
  }

  // 頂点同士を線で結ぶ
  for (int i = 0; i < 12; i++) {

    uint8_t a = edges[i][0];
    uint8_t b = edges[i][1];

    arduboy.drawLine(
      screen[a].x,
      screen[a].y,
      screen[b].x,
      screen[b].y,
      WHITE
    );
  }
}

};

struct Sphere {
  float x,y,z,r=0;
    float scale = 100.0;
Sphere(int x,int y,int z,int r) {
    this->x = x;
  this->y = y;
  this->z = z;
  this->r = r;  
}


  Point3D rotatedXY() {
    Point3D result;
  float x1 = x * cos(angleY) + z * sin(angleY);
  float z1 = -x * sin(angleY) + z * cos(angleY);
    // X軸回転（上下）
  float y2 = y * cos(angleX) - z1 * sin(angleX);

  float z2 = y * sin(angleX) + z1 * cos(angleX);
  
  result.x = x1;
  result.y = y2;
  result.z = z2;

  return result;
  }
  void draw() {
  Point3D rotated=rotatedXY();
  float depth =rotated.z + distance;
  if(depth<0) {depth=0.01;}
 float screenX = 64 + scale * rotated.x / depth;
 float screenY = 32 - scale * rotated.y / depth;
 float screenR=scale*r/depth;
  arduboy.drawCircle(screenX, screenY, screenR, WHITE);
  }
};
Cube court={0,0,0,50,0,25};
Cube a={0,5,0,0.5,5,25};
Cube b={5,5,0,5,5,5};
Sphere s={5,5,0,5};
// 回転速度
const float ROT_SPEED = 0.05;




void update () {
  if(Mode==0) {
      if (arduboy.pressed(LEFT_BUTTON)) {
      angleY -= ROT_SPEED;
    }

    if (arduboy.pressed(RIGHT_BUTTON)) {
      angleY += ROT_SPEED;
    }    
  // 上下回転
  if (arduboy.pressed(UP_BUTTON)) {
    angleX -= ROT_SPEED;
  }

  if (arduboy.pressed(DOWN_BUTTON)) {
    angleX += ROT_SPEED;
  }
  if (arduboy.pressed(A_BUTTON)) {
  if(distance>60) {
    distance-=1;
  }
  }
  if (arduboy.pressed(B_BUTTON)) {
  distance+=1;
  }    
  }else {
          if (arduboy.pressed(LEFT_BUTTON)) {
      b.translate({0,0,-0.5});
    }

    if (arduboy.pressed(RIGHT_BUTTON)) {
      b.translate({0,0,0.5});
    }    
  // 上下回転
  if (arduboy.pressed(UP_BUTTON)) {
    b.translate({-0.5,0,0});
  }

  if (arduboy.pressed(DOWN_BUTTON)) {
    b.translate({0.5,0,0});
  }
  if (arduboy.pressed(A_BUTTON)) {
    distance+=1;
  }
  if (arduboy.pressed(B_BUTTON)) {
    Mode=0;
  }    
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

  update();
  Serial.println(distance);
  // 立方体描画
  court.draw();
  a.draw();
  b.draw();
  s.draw();

  arduboy.display();
}
