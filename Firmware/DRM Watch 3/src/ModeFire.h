#ifndef MODEFIRE_H
#define MODEFIRE_H

/*
Код написано на базі роботи Кирила Лейфера, ось його репозиторій:
https://github.com/BOOtak/dct3ngine/blob/master/src/games/perlin_test
*/



#include "Global.h"
#include "AutoSleep.h"
#include "Button.h"
#include "ScreenBuffer.h"
#include "ModeMenuApps.h"

#define ONE 0x400
#define FRAC_BITS 10
#define FRAC_MASK 0x000003FF
#define MANT_MASK 0xFFFFFC00
#define SIGN_MASK 0xFFC00000


#define FIX_MUL(X, Y) ((int)(((X) * (Y)) >> FRAC_BITS))
#define FIX_DIV(X, Y) ((int)(((X) / ((Y) >> FRAC_BITS))))

#define TO_INT(X) ((X) >> FRAC_BITS)
#define FIX(X) ((int)((X) << FRAC_BITS))
#define correct_angle(a) ((long long)a * ONE / (1 << 30))

int randoms = 0;
int rotations = 0;
int gradients = 0;
int interpolations = 0;
static int x0_cache[2];
static int y0_cache[2];
static Point grid_cache[2][2][2];
int dy_low = FIX(0);
int dy_high = FIX(0);
int angle = FIX(0);

RTC_DATA_ATTR int R_randoms = 0;
RTC_DATA_ATTR int R_rotations = 0;
RTC_DATA_ATTR int R_gradients = 0;
RTC_DATA_ATTR int R_interpolations = 0;
RTC_DATA_ATTR static int R_x0_cache[2];
RTC_DATA_ATTR static int R_y0_cache[2];
RTC_DATA_ATTR static Point R_grid_cache[2][2][2];
RTC_DATA_ATTR int R_dy_low = FIX(0);
RTC_DATA_ATTR int R_dy_high = FIX(0);
RTC_DATA_ATTR int R_angle = FIX(0);

void fireBackupToRtc(){
  for(int i=0; i<2; i++)
    R_x0_cache[i] = x0_cache[i];
  for(int i=0; i<2; i++)
    R_y0_cache[i] = y0_cache[i];
  for(int i=0; i<2; i++)
    for(int j=0; j<2; j++)
      for(int k=0; k<2; k++)
        R_grid_cache[i][j][k] = grid_cache[i][j][k];
  R_randoms = randoms;
  R_rotations = rotations;
  R_gradients = gradients;
  R_interpolations = interpolations;
  R_dy_low = dy_low;
  R_dy_high = dy_high;
  R_angle = angle;
}
void fireRestoreFromRtc(){
  for(int i=0; i<2; i++)
    x0_cache[i] = R_x0_cache[i];
  for(int i=0; i<2; i++)
    y0_cache[i] = R_y0_cache[i];
  for(int i=0; i<2; i++)
    for(int j=0; j<2; j++)
      for(int k=0; k<2; k++)
        grid_cache[i][j][k] = R_grid_cache[i][j][k];
  randoms = R_randoms;
  rotations = R_rotations;
  gradients = R_gradients;
  interpolations = R_interpolations;
  dy_low = R_dy_low;
  dy_high = R_dy_high;
  angle = R_angle;
}


static const int costable[] = {
    correct_angle(1073741824),
    correct_angle(1073578288),
    correct_angle(1073087729),
    correct_angle(1072270298),
    correct_angle(1071126243),
    correct_angle(1069655912),
    correct_angle(1067859754),
    correct_angle(1065738315),
    correct_angle(1063292242),
    correct_angle(1060522280),
    correct_angle(1057429273),
    correct_angle(1054014162),
    correct_angle(1050277989),
    correct_angle(1046221891),
    correct_angle(1041847103),
    correct_angle(1037154959),
    correct_angle(1032146887),
    correct_angle(1026824413),
    correct_angle(1021189159),
    correct_angle(1015242840),
    correct_angle(1008987269),
    correct_angle(1002424350),
    correct_angle(995556083),
    correct_angle(988384560),
    correct_angle(980911966),
    correct_angle(973140576),
    correct_angle(965072759),
    correct_angle(956710970),
    correct_angle(948057759),
    correct_angle(939115760),
    correct_angle(929887697),
    correct_angle(920376381),
    correct_angle(910584710),
    correct_angle(900515665),
    correct_angle(890172315),
    correct_angle(879557810),
    correct_angle(868675383),
    correct_angle(857528349),
    correct_angle(846120104),
    correct_angle(834454122),
    correct_angle(822533958),
    correct_angle(810363241),
    correct_angle(797945680),
    correct_angle(785285058),
    correct_angle(772385229),
    correct_angle(759250125),
    correct_angle(745883746),
    correct_angle(732290163),
    correct_angle(718473518),
    correct_angle(704438018),
    correct_angle(690187940),
    correct_angle(675727625),
    correct_angle(661061475),
    correct_angle(646193961),
    correct_angle(631129609),
    correct_angle(615873009),
    correct_angle(600428808),
    correct_angle(584801711),
    correct_angle(568996477),
    correct_angle(553017922),
    correct_angle(536870912),
    correct_angle(520560366),
    correct_angle(504091252),
    correct_angle(487468587),
    correct_angle(470697435),
    correct_angle(453782903),
    correct_angle(436730145),
    correct_angle(419544355),
    correct_angle(402230767),
    correct_angle(384794656),
    correct_angle(367241333),
    correct_angle(349576144),
    correct_angle(331804471),
    correct_angle(313931728),
    correct_angle(295963357),
    correct_angle(277904834),
    correct_angle(259761657),
    correct_angle(241539355),
    correct_angle(223243478),
    correct_angle(204879599),
    correct_angle(186453311),
    correct_angle(167970228),
    correct_angle(149435979),
    correct_angle(130856211),
    correct_angle(112236583),
    correct_angle(93582766),
    correct_angle(74900443),
    correct_angle(56195305),
    correct_angle(37473049),
    correct_angle(18739379),
    correct_angle(0),
};

int fix_cos(int deg)
{
    int sign = 0;
    int val;
    int i;

    if (deg < FIX(0))
        deg = -deg;
    while (deg >= FIX(360))
        deg -= FIX(360);
    if (FIX(180) - deg <= 0)
        deg = FIX(180) + (FIX(180) - deg);
    if (FIX(90) - deg <= 0)
    {
        deg = FIX(90) + (FIX(90) - deg);
        sign = 1;
    }

    if (deg <= FIX(0))
        val = ONE;
    else if (FIX(90) - deg <= 0)
        val = 0;
    else
    {
        i = deg / ONE;
        val = deg % ONE;
        if (val != 0)
            val = (costable[i] * (ONE - val) + costable[i + 1] * val) / ONE;
        else
            val = costable[i];
    }

    return !sign ? val : -val;
}

int fix_sin(int deg)
{
    return fix_cos(FIX(90) - deg);
}

Point randomGradient(int ix, int iy)
{
    // No precomputed gradients mean this works for any number of grid coordinates
    unsigned a = ix, b = iy;
    a *= 3284157443u;
    b ^= a << 16 | a >> 16;
    b *= 1911520717u;
    a ^= b << 16 | b >> 16;
    a *= 2048419325u;
    int random = FIX_MUL((a >> 22), FIX(360)); // in [0, 360]
    Point v;
    v.x = fix_cos(random);
    v.y = fix_sin(random);
    randoms++;
    return v;
}

// Rotate vector by angle
Point fix_Vector2Rotate(Point v, int angle)
{
    Point result = {0};

    int cosres = fix_cos(angle);
    int sinres = fix_sin(angle);

    result.x = FIX_MUL(v.x, cosres) - FIX_MUL(v.y, sinres);
    result.y = FIX_MUL(v.x, sinres) + FIX_MUL(v.y, cosres);

    rotations++;
    return result;
}


// cubic interpolation
int interpolate(int a0, int a1, int w)
{
    interpolations++;
    return FIX_MUL(FIX_MUL(FIX_MUL((a1 - a0), (FIX(3) - FIX_MUL(w, FIX(2)))), w), w) + a0;
    // return a0 + FIX_MUL(w, (a1 - a0));
}


// Calculate two vectors dot product
int fix_Vector2DotProduct(Point v1, const Point *v2)
{
    int result = (FIX_MUL(v1.x, v2->x) + FIX_MUL(v1.y, v2->y));
    return result;
}
// Computes the dot product of the distance and gradient vectors.
int dotGridGradient(const Point *gradient, int ix, int iy, int x, int y)
{
    // Compute the distance vector
    Point dist;
    dist.x = x - FIX(ix);
    dist.y = y - FIX(iy);

    gradients++;
    int result = fix_Vector2DotProduct(dist, gradient);
    // if (ix == 3 && iy == 2)
    // {
    //     printf("    %d %d: %f %f; %f %f: %f\n", ix, iy, to_float(gradient->x), to_float(gradient->y), to_float(x), to_float(y), to_float(result));
    // }
    return result;
}


// Compute Perlin noise at coordinates x, y
int perlin(int x, int y, int angle, int cache_idx)
{
    // Determine grid cell coordinates
    int x0 = TO_INT(x);
    int x1 = x0 + 1;
    int y0 = TO_INT(y);
    int y1 = y0 + 1;

    // Determine interpolation weights
    // Could also use higher order polynomial/s-curve here
    int sx = x - FIX(x0);
    int sy = y - FIX(y0);

    // Interpolate between grid point gradients
    int n0, n1, ix0, ix1, value;

    if (x0 != x0_cache[cache_idx] || y0 != y0_cache[cache_idx])
    {
        grid_cache[cache_idx][0][0] = fix_Vector2Rotate(randomGradient(x0, y0), angle);
        grid_cache[cache_idx][0][1] = fix_Vector2Rotate(randomGradient(x0, y1), angle);
        grid_cache[cache_idx][1][0] = fix_Vector2Rotate(randomGradient(x1, y0), angle);
        grid_cache[cache_idx][1][1] = fix_Vector2Rotate(randomGradient(x1, y1), angle);
        x0_cache[cache_idx] = x0;
        y0_cache[cache_idx] = y0;
    }

    n0 = dotGridGradient(&grid_cache[cache_idx][0][0], x0, y0, x, y);
    n1 = dotGridGradient(&grid_cache[cache_idx][1][0], x1, y0, x, y);
    ix0 = interpolate(n0, n1, sx);

    n0 = dotGridGradient(&grid_cache[cache_idx][0][1], x0, y1, x, y);
    n1 = dotGridGradient(&grid_cache[cache_idx][1][1], x1, y1, x, y);
    ix1 = interpolate(n0, n1, sx);

    value = interpolate(ix0, ix1, sy);
    return (value + ONE) / 2;
}


void setModeFire(){
  clearScreenAnimation();
  Serial.println(F("Set mode: Fire"));
  modeSetup = setModeFire;
  modeLoop = modeFireLoop;
  modeButtonUp = ModeFireButtonUp;
  modeButtonCenter = ModeFireButtonCenter;
  modeButtonDown = ModeFireButtonDown;
  modeButtonUpLong = 0;
  modeButtonCenterLong = 0;
  modeButtonDownLong = 0;
  registerAction();
  enableAutoReturn = false;
  enableAutoSleep = false; 
  autoReturnTime = autoReturnDefaultTime;
  autoSleepTime = autoSleepDefaultTime;
}

void modeFireLoop(){
  unsigned long millisStarted = millis();
  fireStep();
  //lavaStep();
  unsigned long millisEnd = millis();
  Serial.print("Fire step: "); Serial.print(millisEnd-millisStarted); Serial.println(" ms.");
  drawScreenBuffer();
  lcd()->sendBuffer();
}

void fireStep(){
  fireStep(BUFF_W);
}
void fireStep(int w){  //process not full but left part to W (BUFFER COORDINATES!)
  zeroScreenBuffer();
  fireRestoreFromRtc();
  dy_low += ONE / 6;
  dy_high += ONE / 4;
  int cell_size = 32; //16
  int one_over_screen_height = FIX_DIV(ONE, FIX(BUFF_H));
  for (int j = 0; j < BUFF_H; j+=1) {
    for (int i = 0; i < w - 1; i+=1) {
      if (i + j & 1) {
          int val_low = perlin(FIX(i) / cell_size, FIX(j) / cell_size + dy_low, angle, 0);
          int mul = FIX_MUL(FIX(j), one_over_screen_height);
          val_low = FIX_MUL(val_low, mul);
          int val_high = perlin(FIX(i) / cell_size * 2, FIX(j) / cell_size * 2 + dy_high, angle, 1);
          int res = (val_low + val_high / 2) * 2 / 3;
          if (res > 350)
              setScreenBuffer(i, j, true);
          if (res > 450)
              setScreenBuffer(i+1, j, true);
      }
    }
  }
  fireBackupToRtc();
}


void setModeLava(){
  clearScreenAnimation();
  Serial.println(F("Set mode: Lava"));
  modeSetup = setModeLava;
  modeLoop = modeLavaLoop;
  modeButtonUp = ModeFireButtonUp;
  modeButtonCenter = ModeFireButtonCenter;
  modeButtonDown = ModeFireButtonDown;
  modeButtonUpLong = 0;
  modeButtonCenterLong = 0;
  modeButtonDownLong = 0;
  registerAction();
  enableAutoReturn = false;
  enableAutoSleep = false; 
  autoReturnTime = autoReturnDefaultTime;
  autoSleepTime = autoSleepDefaultTime;
}

void modeLavaLoop(){
  unsigned long millisStarted = millis();
  //fireStep();
  lavaStep();
  unsigned long millisEnd = millis();
  Serial.print("Fire step: "); Serial.print(millisEnd-millisStarted); Serial.println(" ms.");
  drawScreenBuffer();
  lcd()->sendBuffer();
}

void lavaStep(){
  lavaStep(BUFF_W);
}
void lavaStep(int w){  //process not full but left part to W (BUFFER COORDINATES!)
  zeroScreenBuffer();
  fireRestoreFromRtc();
  dy_low += ONE / 30;
  //dy_high += ONE / 40;
  int cell_size = 32; //16
  angle -= FIX(8);
  int one_over_screen_height = FIX_DIV(ONE, FIX(BUFF_H));
  for (int j = 0; j < BUFF_H; j+=1) {
    for (int i = 0; i < w - 1; i+=1) {
      if (i + j & 1) {
          int res = perlin(FIX(i) / cell_size, FIX(j) / cell_size + dy_low, angle, 0);
          if (res > 500)
              setScreenBuffer(i, j, true);
          if (res > 600)
              setScreenBuffer(i+1, j, true);
      }
    }
  }
  fireBackupToRtc();
}


void ModeFireButtonUp(){
  setModeAppsMenu();
}

void ModeFireButtonCenter(){
  setModeAppsMenu();
}

void ModeFireButtonDown(){
  setModeAppsMenu();
}






#endif