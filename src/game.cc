#include "game.h"

#include <string>

#include "player.h"
#include "raylib.h"

namespace game {

Game::Game(unsigned int screen_width, unsigned int screen_height,
           const std::string& title, unsigned int target_fps) noexcept
    : window_{rl::Window(screen_width, screen_height, title,
                         FLAG_WINDOW_RESIZABLE)},
      screen_width_(screen_width),
      screen_height_(screen_height),
      scale_{static_cast<float>(screen_width) / DEFAULT_SCREEN_WIDTH} {
    window_.SetTargetFPS(target_fps);
}

void Game::Run() {
    std::string player_texture_path =
        (resource_path_ / "sprites" / "jenny-walking.png").generic_string();

    rl::Texture player_texture = rl::Texture(player_texture_path);

    Player player(rl::Vector2(static_cast<float>(screen_width_) / 5,
                              static_cast<float>(screen_height_) / 5),
                  std::move(player_texture), rl::Vector2{10, 10});

    player_ = std::make_unique<Player>(std::move(player));

    std::string test_background_texture_path =
        (resource_path_ / "sprites" / "test-background.png").generic_string();

    rl::Texture background_texture = rl::Texture(test_background_texture_path);

    while (!window_.ShouldClose()) {
        if (window_.IsResized()) {
            HandleResize();
            // player_->SetPosition(static_cast<float>(screen_width_) / 2 - 64,
            //                      static_cast<float>(screen_height_) / 2 -
            //                      64);
        }

        HandleKeyboardEvents();

        rl::Rectangle source_rec = {
            0.0F, 0.0F, static_cast<float>(background_texture.GetWidth()),
            static_cast<float>(background_texture.GetHeight())};

        float height = static_cast<float>(window_.GetWidth()) * (9.0F / 16.0F);
        float width = static_cast<float>(height * 16.0F / 9.0F);

        float ratio = static_cast<float>(window_.GetWidth()) /
                      static_cast<float>(window_.GetHeight());

        rl::Rectangle dest_rec = {0.0F, 0.0F,
                                  static_cast<float>(window_.GetWidth()),
                                  static_cast<float>(window_.GetHeight())};

        if (ratio > 16.0F / 9.0F) {
            float width =
                static_cast<float>(window_.GetHeight()) * (16.0F / 9.0F);

            dest_rec.x = (window_.GetWidth() - width) / 2;
            dest_rec.y = 0.0F;
            dest_rec.width = width;

            player_->SetPositionX(dest_rec.x + dest_rec.width / 6);
            player_->SetPositionY(dest_rec.y + dest_rec.height * 3 / 5);

        } else {
            float height =
                static_cast<float>(window_.GetWidth()) * (9.0F / 16.0F);

            dest_rec.x = 0.0F;
            dest_rec.y = (window_.GetHeight() - height) / 2;
            dest_rec.height = height;

            player_->SetPositionX(dest_rec.x + dest_rec.width / 6);
            player_->SetPositionY(dest_rec.y + dest_rec.height * 7 / 10);
        }

        window_.BeginDrawing();

        window_.ClearBackground(SPACE);

        background_texture.Draw(source_rec, dest_rec);

        player_->Draw(scale_);

        dest_rec.DrawLines(rl::Color::RayWhite(), 2.0F);

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
            break;
        default:
            break;
    };
}

}  // namespace game
