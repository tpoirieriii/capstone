#include "game.h"

#include <string>

#include "player.h"
#include "raylib.h"
#include "button.h"
#include "colors.h"


namespace game {

Game::Game(unsigned int screen_width, unsigned int screen_height,
           const std::string& title, unsigned int target_fps) noexcept
    : window_{rl::Window(screen_width, screen_height, title, FLAG_WINDOW_RESIZABLE)},
      screen_width_(screen_width),
      screen_height_(screen_height),
      scale_{static_cast<float>(screen_width) / DEFAULT_SCREEN_WIDTH} {
    window_.SetTargetFPS(target_fps);
}



void Game::Run() {

    std::string player_texture_path =
        (resource_path_ / "sprites" / "jenny-walking.png").generic_string();  // Path to the player texture

    rl::Texture player_texture = rl::Texture(player_texture_path);  // Load the player texture

    Player player(rl::Vector2(static_cast<float>(screen_width_) / 5,
                              static_cast<float>(screen_height_) / 5),
                  std::move(player_texture), rl::Vector2{10, 10});  // Create the player object

    player_ = std::make_unique<Player>(std::move(player));  // Create a unique pointer to the player object

    std::string test_background_texture_path =
        (resource_path_ / "sprites" / "test-background.png").generic_string();  // Path to the background texture

    rl::Texture background_texture = rl::Texture(test_background_texture_path);  // Load the background texture

    // call start screen
    StartScreen();

    // Main game loop
    while (!window_.ShouldClose()) {
        // check to see if we need to change the pause state
        if (IsKeyPressed(KEY_P)) {                                                // If the P key is pressed
            paused_ = !paused_;                                         // Change state to pause screen
        }


        if (window_.IsResized()) {
            HandleResize();
            // player_->SetPosition(static_cast<float>(screen_width_) / 2 - 64,
            //                      static_cast<float>(screen_height_) / 2 -
            //                      64);
        }

        if (paused_){
            HandleKeyboardEvents();
            PauseScreen();  // Draw the pause screen
        };

        rl::Rectangle source_rec = {
            0.0F, 0.0F, static_cast<float>(background_texture.GetWidth()),
            static_cast<float>(background_texture.GetHeight())};  // Source rectangle for the background texture

        float height = static_cast<float>(window_.GetWidth()) * (9.0F / 16.0F);  // Height of the window
        float width = static_cast<float>(height * 16.0F / 9.0F);  // Width of the window

        float ratio = static_cast<float>(window_.GetWidth()) /
                      static_cast<float>(window_.GetHeight());  // Aspect ratio of the window

        rl::Rectangle dest_rec = {0.0F, 0.0F,
                                  static_cast<float>(window_.GetWidth()),
                                  static_cast<float>(window_.GetHeight())};  // Destination rectangle for the background texture

                                  

        if (ratio > 16.0F / 9.0F) {
            float width =
                static_cast<float>(window_.GetHeight()) * (16.0F / 9.0F);  // Width of the window

            dest_rec.x = (window_.GetWidth() - width) / 2;  // X position of the destination rectangle
            dest_rec.y = 0.0F;  // Y position of the destination rectangle
            dest_rec.width = width;  // Width of the destination rectangle

            player_->SetPositionX(dest_rec.x + dest_rec.width / 6);  // Set the player's X position
            player_->SetPositionY(dest_rec.y + dest_rec.height * 3 / 5);  // Set the player's Y position

        } else {
            float height =
                static_cast<float>(window_.GetWidth()) * (9.0F / 16.0F);  // Height of the window

            dest_rec.x = 0.0F;
            dest_rec.y = (window_.GetHeight() - height) / 2;
            dest_rec.height = height;

            player_->SetPositionX(dest_rec.x + dest_rec.width / 6);  // Set the player's X position
            player_->SetPositionY(dest_rec.y + dest_rec.height * 7 / 10);  // Set the player's Y position
        }

        window_.BeginDrawing();

        window_.ClearBackground(SPACE);

        background_texture.Draw(source_rec, dest_rec);

        player_->Draw(scale_);

        dest_rec.DrawLines(rl::Color::RayWhite(), 2.0F);

        DrawText("Game running", 100, 100, 20, WHITE);  // Debugging

        // if we are paused, draw the pause messages and pause the game
        if (paused_) {
            DrawText("Game Paused", 100, 200, 20, WHITE);
            DrawText("Press P to resume", 100, 250, 20, WHITE);

        }

        
        



        // End drawing
        window_.EndDrawing();
    }
}

void Game::HandleResize() {
    screen_width_ = window_.GetWidth();
    screen_height_ = window_.GetHeight();
    scale_ = static_cast<float>(screen_width_) / DEFAULT_SCREEN_WIDTH;
}

rl::Texture LoadTexture(const fs::path& texture_path) {
    return rl::Texture(texture_path.generic_string());
}

/**
 * @brief Handle keyboard events, for example arrow keys, space bar, etc.
 */
void Game::HandleKeyboardEvents() {

    int key = rl::Keyboard::GetKeyPressed();
    switch (key) {
        // Move player left
        case KEY_A:
            player_->Move(Player::Direction::kLeft);
            break;

        // Move player right
        case KEY_D:
            player_->Move(Player::Direction::kRight);
            break;   

        // Inventory
        case KEY_E:
            break;

        // Pause
        case KEY_P:
        if (IsKeyPressed(KEY_P)) {                                                   // If the P key is pressed
            PauseScreen();                                // Debugging
            }
        }
        
    }


void Game::StartScreen() {

    // This will draw the start screen

    static Button start_button(screen_width_ / 2 - 100, this->screen_height_ / 2 - 50, 200, 100, "Start");
    static Button quit_button(screen_width_ / 2 - 100, screen_height_ / 2 + 100, 200, 100, "Quit");

    while (start_button.WasClicked() == false && quit_button.WasClicked() == false)
    {

        HandleKeyboardEvents();

       
    rl::Vector2 mouse_point = GetMousePosition();

    start_button.Update(mouse_point);
    quit_button.Update(mouse_point);
        window_.BeginDrawing();
    window_.ClearBackground(SPACE);

    DrawText("Press Enter to start", 100, 100, 20, WHITE);

    start_button.Draw();
    quit_button.Draw();

    window_.EndDrawing();
    
   if (start_button.WasClicked()) {
        return;
    } else if (quit_button.WasClicked()) {
        window_.Close();
    } else if (IsKeyPressed(KEY_ENTER)) {
        return;
    }
    

    }
}
    



void Game::PauseScreen() {
    // Draw output to make sure the pause screen is being rendered
    std::cout << "Rendering Pause screen" << std::endl;


    // This will handle the input for pausing and resuming the game
   

    static Button resume_button(screen_width_ / 2 - 100, screen_height_ / 2 - 50, 200, 100, "Resume");
    static Button quit_button(screen_width_ / 2 - 100, screen_height_ / 2 + 100, 200, 100, "Quit");

    rl::Vector2 mouse_point = GetMousePosition();

    resume_button.Update(mouse_point);
    quit_button.Update(mouse_point);

    // This will draw the pause screen
    window_.BeginDrawing();
    
    DrawText("Game Paused", 100, 100, 20, WHITE);
    resume_button.Draw();
    quit_button.Draw();


    // Handle mouse input separate from keyboard input
    if (resume_button.WasClicked()) {
        paused_ = false;
    } else if (quit_button.WasClicked()) {
        window_.Close();
    }

    
}

void Game::EndScreen() {
    // TODO
}
} // namespace game