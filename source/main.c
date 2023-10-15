#include <raylib.h>
#include <stdlib.h>
#include <time.h>

typedef struct // ���y �c�y���
{
    float x; // X����
    float y; // Y����
    float radius; // �b�|
    Color color; // �C��
    float speedX; // X�t��
    float speedY; // Y�t��
}Ball;

typedef struct // �y�� �c�y���
{
    float x; // X����
    float y; // Y����
    float width; // �e
    float height; // ��
    Color color; // �C��
    float speed; // �t��
}Bat;

int main() {
    InitWindow(1200, 800, "Ping Pong Game"); // ����
    SetWindowState(FLAG_VSYNC_HINT); // �]�mfps

    srand((unsigned int)time(NULL));

    // * ��l��
    Ball ball = { GetScreenWidth() / 2.0f , GetScreenHeight() / 2.0f , 12 , WHITE , 400 , 250 }; // ���y
    Bat batLeft = { 50 , GetScreenHeight() / 2 - 50 , 15 , 125 ,RED , 700}; // 1�����a�y��
    Bat batRight = { GetScreenWidth() - 50 - 15 , GetScreenHeight() / 2 - 50 , 15 , 125 , BLUE , 700 }; // 2�����a�y��

    // * ��r
    const char* winnerText = NULL;
    const char* restartText = NULL;
    const char* continueText = NULL;

    int ballFlag = 1; // �P�_�o�y��V
    int leftFlag = 0; // �P�_���誱�a�O�_�o��
    int rightFlag = 0; // �P�_�k�誱�a�O�_�o��
    int musicFlag = 0; // �P�_�O�_�n���񭵼�

    // * �[������
    InitAudioDevice();
    Music winnerMusic = LoadMusicStream("source/winner.mp3");
    Music cheersMusic = LoadMusicStream("source/cheers.mp3");

    // * ����
    int scoreLeft = 0;
    int scoreRight = 0;
    int scoreMax = 11;

    const char* playerLeftText = NULL;
    const char* playerRightText = NULL;

    while (!WindowShouldClose())
    {
        /* ******************************* ø�s ******************************* */
        BeginDrawing();
            ClearBackground(BLACK);

            DrawCircle((int)ball.x, (int)ball.y, ball.radius, ball.color); // ø�s�y

            DrawRectangle(batLeft.x, batLeft.y, batLeft.width, batLeft.height, batLeft.color); // ø�s1�����a�y��
            DrawRectangle(batRight.x, batRight.y, batRight.width, batRight.height, batRight.color); // ø�s2�����a�y��

            DrawText(TextFormat("%d", scoreLeft), 20, 50, 35, WHITE);
            DrawText(TextFormat("%d", scoreRight), GetScreenWidth() - 35, 50, 35, WHITE);

            if (continueText) // ��� ��/�k ���a�o����r
            {
                if (leftFlag)
                    DrawText(playerLeftText, GetScreenWidth() / 2 - 400, GetScreenHeight() / 2 - 40, 60, YELLOW);
                if (rightFlag)
                    DrawText(playerRightText, GetScreenWidth() / 2 - 400, GetScreenHeight() / 2 - 40, 60, YELLOW);

                DrawText(continueText, GetScreenWidth() / 2 - 250, GetScreenHeight() / 2 + 50, 40, GREEN);
                PlayMusicStream(cheersMusic);
            }

            if (continueText && IsKeyPressed(KEY_SPACE)) // ���ť����~�����
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

            // * �����Ӫ�
            if (winnerText)
            {
                DrawText(winnerText, GetScreenWidth() / 2 - 250, GetScreenHeight() / 2 - 40, 60, YELLOW);
                DrawText(restartText, GetScreenWidth() / 2 - 250, GetScreenHeight() / 2 + 50, 40, GREEN);

                PlayMusicStream(winnerMusic);
            }
            if (winnerText && IsKeyPressed(KEY_R)) // ���s����
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

            DrawFPS(10, 10); // ø�sfps
        EndDrawing();

        UpdateMusicStream(winnerMusic);
        UpdateMusicStream(cheersMusic);

        /* ******************************* �y��V ******************************* */
        if (ballFlag) // �y�����o
        {
            ball.x -= ball.speedX * GetFrameTime();
            ball.y += ball.speedY * GetFrameTime();
        }
        else // �y���k�o
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

        /* ******************************* ������ ******************************* */

        // * ���a1 left
        if (IsKeyDown(KEY_W) && batLeft.y > 0)
        {
            batLeft.y -= batLeft.speed * GetFrameTime();
        }
        if (IsKeyDown(KEY_S) && batLeft.y < GetScreenHeight() - 125)
        {
            batLeft.y += batLeft.speed * GetFrameTime();
        }

        // * ���a2 right
        if (IsKeyDown(KEY_UP) && batRight.y > 0)
        {
            batRight.y -= batRight.speed * GetFrameTime();
        }
        if (IsKeyDown(KEY_DOWN) && batRight.y < GetScreenHeight() - 125)
        {
            batRight.y += batRight.speed * GetFrameTime();
        }

        /* ******************************* �����O�_�I�� ******************************* */
        if (ballFlag)
        {
            if (ball.x <= batLeft.x + batLeft.width && batLeft.x <= ball.x + ball.radius && ball.y < batLeft.y + batLeft.height && batLeft.y <= ball.y + ball.radius)
            {
                if (ball.speedX > 0)
                {
                    ball.speedX *= -1.1f; // �H�Ϥ�V�ϼu �üW�[�t��
                    ball.speedY = (ball.y - batLeft.y) / (batLeft.height / 2) * -ball.speedX;
                }
            }
            if (ball.x <= batRight.x + batRight.width && batRight.x <= ball.x + ball.radius && ball.y < batRight.y + batRight.height && batRight.y <= ball.y + ball.radius)
            {
                if (ball.speedX < 0)
                {
                    ball.speedX *= -1.1f; // �H�Ϥ�V�ϼu �üW�[�t��
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
                    ball.speedX *= -1.1f; // �H�Ϥ�V�ϼu �üW�[�t��
                    ball.speedY = (ball.y - batLeft.y) / (batLeft.height / 2) * ball.speedX;
                }
            }
            if (ball.x <= batRight.x + batRight.width && batRight.x <= ball.x + ball.radius && ball.y < batRight.y + batRight.height && batRight.y <= ball.y + ball.radius)
            {
                if (ball.speedX > 0)
                {
                    ball.speedX *= -1.1f; // �H�Ϥ�V�ϼu �üW�[�t��
                    ball.speedY = (ball.y - batRight.y) / (batRight.height / 2) * -ball.speedX;
                }
            }
        }

        /* ******************************* �P�_��Ĺ ******************************* */
            
        // * ���a1 left
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

        // * ���a2 right
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

    UnloadMusicStream(winnerMusic); // ���񤺦s
    UnloadMusicStream(cheersMusic); // ���񤺦s
    CloseAudioDevice(); // �������ֳ]��

    CloseWindow();
    return 0;
}