#include "primlib.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUMBER_OF_STARS 5
#define NUMBER_OF_TARGETS 3
#define BULLET_SPEED 5
#define NUMBER_OF_PLAYER_BULLETS 3
#define PLAYER_HEALTH_POINTS 3
#define DISTANCE_CENTER_TO_TOP_SHIP 25
struct enemy_bullets {
  int x;
  int y;
  int speed;
  int hit;
};

struct bullets {
  int x;
  int y;
  int is_shooting;
  int speed;
  int hit;
};

struct stars {
  int x;
  int y;
  int visible;
};

struct target {
  int x;
  int y;
  int speed_x;
  int speed_y;
  int hit;
  int distance;
  char color;
  int is_shooting;
  int bullet_ready;
};

struct ship {
  int x;
  int y;
  int hp;
  int distance;
  int score;
};

void stars_posistion(struct stars *ps) {
  ps->x = rand() % gfx_screenWidth();
  ps->y = rand() % gfx_screenHeight();
  ps->visible = rand() % 2;
}

void draw_screen() {
  gfx_filledRect(0, 0, gfx_screenWidth() - 1, gfx_screenHeight() - 1, BLACK);
}

void draw_score(struct ship player) {
  char score[100];
  snprintf(score, sizeof(score), "Score: %d", player.score);
  gfx_textout(10, 10, score, WHITE);
}

void draw_player_health(struct ship player) {
  char health[10];
  snprintf(health, sizeof(health), "Health: %d", player.hp);
  gfx_textout(10, 20, health, WHITE);
}

void draw_stars(struct stars s) {
  if (s.visible)
    gfx_filledCircle(s.x, s.y, 3, WHITE);
}

void draw_target(struct target t) {
  gfx_filledCircle(t.x, t.y - 12, 10, RED);                      // engine fire
  gfx_filledCircle(t.x, t.y - 12, 8, YELLOW);                    // enginefire
  gfx_filledCircle(t.x, t.y - 12, 6, RED);                       // enginefire
  gfx_filledCircle(t.x, t.y - 12, 4, YELLOW);                    // enginefire
  gfx_filledRect(t.x - 10, t.y + 12, t.x + 10, t.y - 12, WHITE); // body
  gfx_filledTriangle(t.x, t.y + 30, t.x - 10, t.y + 13, t.x + 10, t.y + 13,
                     t.color); // front
  gfx_filledTriangle(t.x - 35, t.y - 6, t.x - 10, t.y + 10, t.x - 10, t.y - 6,
                     t.color); // left wing
  gfx_filledTriangle(t.x + 35, t.y - 6, t.x + 10, t.y + 10, t.x + 10, t.y - 6,
                     t.color);         // right wing
  gfx_filledCircle(t.x, t.y, 6, BLUE); // window
}

void draw_bullet(struct bullets b) {
  if (b.is_shooting && !b.hit) {
    gfx_filledTriangle(b.x, b.y - 20, b.x - 10, b.y, b.x + 10, b.y, RED);
    gfx_filledTriangle(b.x, b.y - 15, b.x - 10, b.y, b.x + 10, b.y, YELLOW);
    gfx_filledTriangle(b.x, b.y - 10, b.x - 10, b.y, b.x + 10, b.y, RED);
    gfx_filledTriangle(b.x, b.y - 5, b.x - 10, b.y, b.x + 10, b.y, YELLOW);
  }
}

void draw_enemy_bullet(struct enemy_bullets b, struct target t) {
  if (t.is_shooting && t.bullet_ready)
    gfx_filledCircle(b.x, b.y, 10, RED);
}

void draw_ship(struct ship p) {
  gfx_filledCircle(p.x, p.y, 15, RED);
  gfx_filledCircle(p.x, p.y, 13, YELLOW);
  gfx_filledCircle(p.x, p.y, 11, RED);
  gfx_filledCircle(p.x, p.y, 9, YELLOW);
  gfx_filledTriangle(p.x, p.y - DISTANCE_CENTER_TO_TOP_SHIP, p.x + 25, p.y, p.x - 25, p.y, WHITE);
  gfx_filledTriangle(p.x, p.y - 20, p.x + 20, p.y - 2, p.x - 20, p.y - 2,
                     MAGENTA);
  gfx_filledTriangle(p.x, p.y - 15, p.x + 15, p.y - 2, p.x - 15, p.y - 2,
                     BLACK);
  gfx_filledTriangle(p.x, p.y - 10, p.x + 10, p.y - 4, p.x - 10, p.y - 4,
                     MAGENTA);
  gfx_filledCircle(p.x, p.y - 7, 3, CYAN);
}

void draw_explosion(const struct target *pt, int *boom_radius,
                    int *boom_color) {
  if (pt->hit)
    gfx_filledCircle(pt->x, pt->y, *boom_radius, *boom_color);
}

void draw_game_over() {
  gfx_filledRect(0, 0, gfx_screenWidth() - 1, gfx_screenHeight() - 1, BLACK);
  gfx_textout((gfx_screenWidth() / 2) - 20, (gfx_screenHeight() / 2) - 10, "GAME OVER",
              WHITE);
  gfx_textout((gfx_screenWidth() / 2) - 50, gfx_screenHeight() / 2 + 10,
              "PRESS ESC TO EXIT", WHITE);
}

void distance_to_target(struct target *pt, struct bullets b) {
  if (b.is_shooting) {
    double x_distance = b.x - pt->x;
    double y_distance = b.y - pt->y;
    pt->distance = hypot((x_distance), (y_distance));
  }
}

void distance_to_player(struct ship *pp, struct enemy_bullets b,
                        struct target t) {
  if (t.is_shooting) {
    double x_distance = b.x - pp->x;
    double y_distance = b.y - pp->y;
    pp->distance = hypot((x_distance), (y_distance));
  }
}

void hit_player(struct ship *pp, struct target *pt) {
  if (pt->is_shooting) {
    if (pp->distance < 20) {
      pp->hp--;
      pt->is_shooting = 0;
    }
  }
}

void enemy_shooting(struct enemy_bullets *pb, struct target *t,
                    struct ship player) {
  if ((!t->is_shooting) && (!t->hit) && (t->x > player.x - 20) &&
      (t->x < player.x + 20)) {
    t->is_shooting = 1;
  }
}

void move_enemy_bullet(struct enemy_bullets *pb, struct target t) {
  if (t.is_shooting)
    pb->y += pb->speed;
}

void enemy_bullet_posistion(struct enemy_bullets *pb, struct target *t) {
  if (!t->is_shooting) {
    pb->x = t->x;
    pb->y = t->y + 30;
    pb->speed = BULLET_SPEED;
    t->bullet_ready = 1;
  }
}

void hit(struct target *pt, struct bullets *pb, struct ship *player) {
  if (pb->is_shooting) {
    if (pt->distance < 30 && !pt->hit) {
      pt->hit = 1;
      pb->hit = 1;
      pb->speed = 0;
      player->score += 1;
      if ((player->score % 10 == 0) && (player->score > 0) && (player->hp < PLAYER_HEALTH_POINTS))
        player->hp++;
    }
  }
}

int random_x() { return rand() % gfx_screenWidth(); }

int random_y() { return rand() % ((gfx_screenHeight() / 2) - 10); }

void explosion(struct target *pt, int *boom_radius, int *boom_color,
               const int *min_boom_radius, const int *max_boom_radius,
               int *boom, struct bullets *pb) {
  if (pt->hit && pb->hit) {
    pt->speed_x = 0;
    pt->speed_y = 0;
    pb->y = -50;
    pb->is_shooting = 0;
    if ((*boom_radius > *min_boom_radius) && (*boom_radius < *max_boom_radius)) {
      if (*boom_color < 7)
        (*boom_color)++;
      else
        *boom_color = 0;
    }
    if (*boom_radius == *max_boom_radius)
      *boom = 1;

    else if (!(*boom))
      (*boom_radius)++;

    if ((*boom == 1) && (*boom_radius > *min_boom_radius)) {
      (*boom_radius)--;
    } else if ((*boom == 1) && (*boom_radius == *min_boom_radius)) {
      pt->x = random_x();
      pt->y = random_y();
      pt->hit = 0;
      pb->hit = 0;
      pt->speed_x = 5;
      pt->speed_y = 3;
      *boom = 0;
      *boom_radius = 0;
    }
  }
}

void move_target(struct target *pt) {
  if ((pt->x >= gfx_screenWidth()) || (pt->x <= 0))
    pt->speed_x *= -1;

  if ((pt->y > (gfx_screenHeight() / 2)) || (pt->y <= 0))
    pt->speed_y *= -1;

  if (pt->x == (rand() % (gfx_screenWidth())))
    pt->speed_x *= -1;

  if (pt->y == (rand() % (gfx_screenHeight() / 2)))
    pt->speed_y *= -1;

  pt->x += pt->speed_x;
  pt->y += pt->speed_y;
}

void move_bullet(struct bullets *pb) {
  if (pb->is_shooting) {
    pb->y -= pb->speed;
  }
}

int main() {
  if (gfx_init())
    exit(3);

  const int max_boom_radius = 30;
  const int min_boom_radius = 0;

  int space_pressed = 0;
  int boom_radius = 0;
  int boom = 0;
  int boom_color = 0;

  srand(time(NULL));

  struct stars star[NUMBER_OF_STARS];
  struct target targets[NUMBER_OF_TARGETS];
  struct bullets bullet[NUMBER_OF_PLAYER_BULLETS] = {0};
  struct enemy_bullets enemy_bullet[NUMBER_OF_TARGETS] = {0};
  struct ship player;

  player.x = gfx_screenWidth() / 2;
  player.y = gfx_screenHeight() / 2 + 300;
  player.hp = PLAYER_HEALTH_POINTS;
  player.score = 0;

  targets[0].x = random_x();
  targets[0].y = random_y();
  targets[0].speed_x = 5;
  targets[0].speed_y = 3;
  targets[0].hit = 0;
  targets[0].color = MAGENTA;
  targets[0].is_shooting = 0;

  targets[1].x = random_x();
  targets[1].y = random_y();
  targets[1].speed_x = 5;
  targets[1].speed_y = 3;
  targets[1].hit = 0;
  targets[1].color = CYAN;
  targets[1].is_shooting = 0;

  targets[2].x = random_x();
  targets[2].y = random_y();
  targets[2].speed_x = 5;
  targets[2].speed_y = 3;
  targets[2].hit = 0;
  targets[2].color = GREEN;
  targets[2].is_shooting = 0;
  
  for (int i = 0; i < sizeof(star) / sizeof(*star); i++)
        stars_posistion(&star[i]);

  while (1) {
    if (player.hp <= 0) {
      draw_game_over();
      gfx_updateScreen();

      if (gfx_isKeyDown(SDLK_ESCAPE))
        break;
    } else {
      // Drawing
      draw_screen();
      draw_score(player);
      draw_player_health(player);
      for (int i = 0; i < sizeof(star) / sizeof(*star); i++)
        draw_stars(star[i]);

      draw_ship(player);

      for (int i = 0; i < sizeof(targets) / sizeof(*targets); i++) {
        draw_enemy_bullet(enemy_bullet[i], targets[i]);
        draw_target(targets[i]);
        draw_explosion(&targets[i], &boom_radius, &boom_color);
      }

      for (int i = 0; i < sizeof(bullet) / sizeof(*bullet); i++)
        draw_bullet(bullet[i]);

      gfx_updateScreen();

      // Variables

      for (int i = 0; i < sizeof(star) / sizeof(*star); i++)
        stars_posistion(&star[i]);

      for (int i = 0; i < sizeof(targets) / sizeof(*targets); i++) {
        move_target(&targets[i]);
        enemy_bullet_posistion(&enemy_bullet[i], &targets[i]);
        enemy_shooting(&enemy_bullet[i], &targets[i], player);
        move_enemy_bullet(&enemy_bullet[i], targets[i]);
        distance_to_player(&player, enemy_bullet[i], targets[i]);
        hit_player(&player, &targets[i]);
        if (enemy_bullet[i].y >= gfx_screenHeight())
          targets[i].is_shooting = 0;
      }

      for (int i = 0; i < sizeof(bullet) / sizeof(*bullet); i++) {
        move_bullet(&bullet[i]);
        if (bullet[i].y <= 0 || bullet[i].y >= gfx_screenHeight())
          bullet[i].is_shooting = 0;
      }

      for (int i = 0; i < sizeof(targets) / sizeof(*targets); i++) {
        for (int j = 0; j < sizeof(bullet) / sizeof(*bullet); j++) {
          distance_to_target(&targets[i], bullet[j]);
          hit(&targets[i], &bullet[j], &player);
          explosion(&targets[i], &boom_radius, &boom_color, &min_boom_radius,
                    &max_boom_radius, &boom, &bullet[j]);
        }
      }

      // Steering
      if ((gfx_isKeyDown(SDLK_RIGHT)) && (player.x <= gfx_screenWidth()))
        player.x += 3;
      if ((gfx_isKeyDown(SDLK_LEFT) && player.x >= 0))
        player.x -= 3;
      if (gfx_isKeyDown(SDLK_SPACE)) {
        if (!space_pressed) {
          for (int i = 0; i < NUMBER_OF_PLAYER_BULLETS; i++) {
            if (!bullet[i].is_shooting && !bullet[i].hit) {
              bullet[i].hit = 0;
              bullet[i].x = player.x;
              bullet[i].y = player.y - DISTANCE_CENTER_TO_TOP_SHIP;
              bullet[i].speed = BULLET_SPEED;
              bullet[i].is_shooting = 1;
              break;
            }
          }
        }
        space_pressed = 1;
      } else {
        space_pressed = 0;
      }
      if (gfx_isKeyDown(SDLK_ESCAPE))
        break;
      SDL_Delay(10);
    };
  }
  return 0;
}
