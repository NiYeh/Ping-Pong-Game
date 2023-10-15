#include <raylib.h>
#include <stdlib.h>
#include <time.h>

typedef struct // 乒乓球 構造函數
{
    float x; // X坐標
    float y; // Y坐標
    float radius; // 半徑
    Color color; // 顏色
    float speedX; // X速度
    float speedY; // Y速度
}Ball;

typedef struct // 球拍 構造函數
{
    float x; // X坐標
    float y; // Y坐標
    float width; // 寬
    float height; // 高
    Color color; // 顏色
    float speed; // 速度
}Bat;

int main() {
    InitWindow(1200, 800, "Ping Pong Game"); // 視窗
    SetWindowState(FLAG_VSYNC_HINT); // 設置fps

    srand((unsigned int)time(NULL));

    // * 初始化
    Ball ball = { GetScreenWidth() / 2.0f , GetScreenHeight() / 2.0f , 12 , WHITE , 400 , 250 }; // 乒乓球
    Bat batLeft = { 50 , GetScreenHeight() / 2 - 50 , 15 , 125 ,RED , 700}; // 1號玩家球拍
    Bat batRight = { GetScreenWidth() - 50 - 15 , GetScreenHeight() / 2 - 50 , 15 , 125 , BLUE , 700 }; // 2號玩家球拍

    // * 文字
    const char* winnerText = NULL;
    const char* restartText = NULL;
    const char* continueText = NULL;

    int ballFlag = 1; // 判斷發球方向
    int leftFlag = 0; // 判斷左方玩家是否得分
    int rightFlag = 0; // 判斷右方玩家是否得分
    int musicFlag = 0; // 判斷是否要播放音樂

    // * 加載音樂
    InitAudioDevice();
    Music winnerMusic = LoadMusicStream("source/winner.mp3");
    Music cheersMusic = LoadMusicStream("source/cheers.mp3");

    // * 分數
    int scoreLeft = 0;
    int scoreRight = 0;
    int scoreMax = 11;

    const char* playerLeftText = NULL;
    const char* playerRightText = NULL;

    while (!WindowShouldClose())
    {
        /* ******************************* 繪製 ******************************* */
        BeginDrawing();
            ClearBackground(BLACK);

            DrawCircle((int)ball.x, (int)ball.y, ball.radius, ball.color); // 繪製球

            DrawRectangle(batLeft.x, batLeft.y, batLeft.width, batLeft.height, batLeft.color); // 繪製1號玩家球拍
            DrawRectangle(batRight.x, batRight.y, batRight.width, batRight.height, batRight.color); // 繪製2號玩家球拍

            DrawText(TextFormat("%d", scoreLeft), 20, 50, 35, WHITE);
            DrawText(TextFormat("%d", scoreRight), GetScreenWidth() - 35, 50, 35, WHITE);

            if (continueText) // 顯示 左/右 玩家得分文字
            {
                if (leftFlag)
                    DrawText(playerLeftText, GetScreenWidth() / 2 - 400, GetScreenHeight() / 2 - 40, 60, YELLOW);
                if (rightFlag)
                    DrawText(playerRightText, GetScreenWidth() / 2 - 400, GetScreenHeight() / 2 - 40, 60, YELLOW);

                DrawText(continueText, GetScreenWidth() / 2 - 250, GetScreenHeight() / 2 + 50, 40, GREEN);
                PlayMusicStream(cheersMusic);
            }

            if (continueText && IsKeyPressed(KEY_SPACE)) // 按空白鍵繼續比賽
            {
                continueText = NULL;
                playerLeftText = NULL;
                playerRightText = NULL;

                leftFlag = 0;
                rightFlag = 0;

                ball.x = GetScreenWidth() / 2.0f;
                ball.y = GetScreenHeight() / 2.0f;
                ball.speedX = 400;
                ball.speedY = 250;

                StopMusicStream(cheersMusic);

                if (ballFlag)
                    ballFlag = 0;
                else
                    ballFlag = 1;
            }

            // * 顯示獲勝者
            if (winnerText)
            {
                DrawText(winnerText, GetScreenWidth() / 2 - 250, GetScreenHeight() / 2 - 40, 60, YELLOW);
                DrawText(restartText, GetScreenWidth() / 2 - 250, GetScreenHeight() / 2 + 50, 40, GREEN);

                PlayMusicStream(winnerMusic);
            }
            if (winnerText && IsKeyPressed(KEY_R)) // 重新比賽
            {
                ball.x = GetScreenWidth() / 2.0f;
                ball.y = GetScreenHeight() / 2.0f;
                ball.speedX = 400;
                ball.speedY = 250;
                winnerText = NULL;
                restartText = NULL;
                scoreLeft = 0;
                scoreRight = 0;
                ballFlag = 1;

                StopMusicStream(winnerMusic);
            }

            DrawFPS(10, 10); // 繪製fps
        EndDrawing();

        UpdateMusicStream(winnerMusic);
        UpdateMusicStream(cheersMusic);

        /* ******************************* 球方向 ******************************* */
        if (ballFlag) // 球往左發
        {
            ball.x -= ball.speedX * GetFrameTime();
            ball.y += ball.speedY * GetFrameTime();
        }
        else // 球往右發
        {
            ball.x += ball.speedX * GetFrameTime();
            ball.y += ball.speedY * GetFrameTime();
        }

        if (ball.y < 0)
        {
            ball.y = 0;
            ball.speedY *= -1;
        }
        if (ball.y > GetScreenHeight())
        {
            ball.y = GetScreenHeight();
            ball.speedY *= -1;
        }

        /* ******************************* 控制鍵 ******************************* */

        // * 玩家1 left
        if (IsKeyDown(KEY_W) && batLeft.y > 0)
        {
            batLeft.y -= batLeft.speed * GetFrameTime();
        }
        if (IsKeyDown(KEY_S) && batLeft.y < GetScreenHeight() - 125)
        {
            batLeft.y += batLeft.speed * GetFrameTime();
        }

        // * 玩家2 right
        if (IsKeyDown(KEY_UP) && batRight.y > 0)
        {
            batRight.y -= batRight.speed * GetFrameTime();
        }
        if (IsKeyDown(KEY_DOWN) && batRight.y < GetScreenHeight() - 125)
        {
            batRight.y += batRight.speed * GetFrameTime();
        }

        /* ******************************* 偵測是否碰撞 ******************************* */
        if (ballFlag)
        {
            if (ball.x <= batLeft.x + batLeft.width && batLeft.x <= ball.x + ball.radius && ball.y < batLeft.y + batLeft.height && batLeft.y <= ball.y + ball.radius)
            {
                if (ball.speedX > 0)
                {
                    ball.speedX *= -1.1f; // 以反方向反彈 並增加速度
                    ball.speedY = (ball.y - batLeft.y) / (batLeft.height / 2) * -ball.speedX;
                }
            }
            if (ball.x <= batRight.x + batRight.width && batRight.x <= ball.x + ball.radius && ball.y < batRight.y + batRight.height && batRight.y <= ball.y + ball.radius)
            {
                if (ball.speedX < 0)
                {
                    ball.speedX *= -1.1f; // 以反方向反彈 並增加速度
                    ball.speedY = (ball.y - batRight.y) / (batRight.height / 2) * ball.speedX;
                }
            }
        }

        else
        {
            if (ball.x <= batLeft.x + batLeft.width && batLeft.x <= ball.x + ball.radius && ball.y < batLeft.y + batLeft.height && batLeft.y <= ball.y + ball.radius)
            {
                if (ball.speedX < 0)
                {
                    ball.speedX *= -1.1f; // 以反方向反彈 並增加速度
                    ball.speedY = (ball.y - batLeft.y) / (batLeft.height / 2) * ball.speedX;
                }
            }
            if (ball.x <= batRight.x + batRight.width && batRight.x <= ball.x + ball.radius && ball.y < batRight.y + batRight.height && batRight.y <= ball.y + ball.radius)
            {
                if (ball.speedX > 0)
                {
                    ball.speedX *= -1.1f; // 以反方向反彈 並增加速度
                    ball.speedY = (ball.y - batRight.y) / (batRight.height / 2) * -ball.speedX;
                }
            }
        }

        /* ******************************* 判斷輸贏 ******************************* */
            
        // * 玩家1 left
        if (ball.x > GetScreenWidth() && !leftFlag)
        {
            scoreLeft += 1;
            leftFlag = 1;

            if (scoreLeft >= scoreMax)
            {
                winnerText = "Left Player Win !!!";
                restartText = "Press R to restart";
            }
            else
            {
                continueText = "Press SPACE to restart";
                playerLeftText = "Left player scores 1 point !";
            }
        }

        // * 玩家2 right
        if (ball.x < 0 && !rightFlag)
        {
            scoreRight += 1;
            rightFlag = 1;

            if (scoreRight >= scoreMax)
            {
                winnerText = "Right Player Win !!!";
                restartText = "Press R to restart";
            }
            else
            {
                continueText = "Press SPACE to restart";
                playerRightText = "Right player scores 1 point !";
            }
        }
    }

    UnloadMusicStream(winnerMusic); // 釋放內存
    UnloadMusicStream(cheersMusic); // 釋放內存
    CloseAudioDevice(); // 關閉音樂設備

    CloseWindow();
    return 0;
}