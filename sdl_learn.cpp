#include "Includs.h"
#include "MySDL.h"
#include "Sprite.h"
#include "InitError.h"
#include "TextureText.h"
#include "LButton.h"

static SDL_Window* window = nullptr;
static SDL_Renderer* render = nullptr;


void Init(Uint8 flags)
{
    if (SDL_Init(flags) != 0)
        throw InitError();
    if (!TTF_WasInit())
    {
        if (TTF_Init() < 0)
        {
            std::cout << "SDL_ttf could not init: " << TTF_GetError() << std::endl;
        }
    }


    SDL_DisplayMode displayMode;
    if (SDL_GetDesktopDisplayMode(0, &displayMode))
        throw InitError();
    window = SDL_CreateWindow("Capybara Ross", 0, 0, displayMode.w, displayMode.h, SDL_WINDOW_BORDERLESS);
    if (window == nullptr)
        throw InitError();
    render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (window == nullptr)
        throw InitError();
    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
}


auto updateFrame = [](SDL _backscreen, SDL _btn, TextureText _mainText, std::string text)
    {
        SDL_RenderClear(render);
        _backscreen.copyRend();
        _btn.copyRend();
        _mainText.setText(text, 400);
        _mainText.render(100, 100);
    };

// в общем походу надо делать фон сразу в классе с отрисовкой переднего плана (анимацией в этом случае)
//13 кадр = выстрел
enum eKeyPressSurface
{
    KEY_PRESS_SURFACE_DEFAULT,
    KEY_PRESS_SURFACE_1,
    KEY_PRESS_SURFACE_2,
    KEY_PRESS_SURFACE_3,
    KEY_PRESS_SURFACE_TOTAL
};

Mix_Chunk* loadSound(std::string name)
{
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        std::cout << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
    }

    //Load music
    Mix_Chunk* gMusic = Mix_LoadWAV(name.c_str());
    if (gMusic == NULL)
    {
        std::cout << "Failed to load beat music! SDL_mixer Error: " << Mix_GetError() << std::endl;
    }
    return gMusic;
}

void animation(Sprite& new_spr,  size_t count_frame, int num_of_frame_sound)
{
    int frame = 0;
    while (frame < count_frame)
    {
        new_spr.renderer(100, 0, frame);
        frame++;
        Sleep(200);
        //Update screen
        if (frame == num_of_frame_sound && new_spr.getAudio() != nullptr)
        {
            if(Mix_PlayChannel(-1, new_spr.getAudio(), 1) == -1)
            {
                std::cout << "Mix_PlayChannel: " << Mix_GetError() << std::endl;
            }
        }
        SDL_RenderPresent(new_spr.getRender());
    }
}



// blank - холостой
// live - заряженный
// mind - рассудок; восполняется энергетиками и сигаретами
// amount - количество

class Member {
protected:
    int loses_count;

public:
    Member() { loses_count = 0; }
    virtual int get_loses_count() = 0;
    virtual void add_loses_count() = 0;
    virtual int get_hp() = 0;
};
class Player : public Member {
private:
    int mind;
    int hp;
    int loses_count;

public:
    Player() {
        hp = 0;
        mind = 100;
        loses_count = 0;
    }
    void receive_damage(bool is_hitted) {
        if (is_hitted) {
            hp--;
            mind -= 10;
        }
    }
    void healhp(bool energy_drink) {
        if (energy_drink) {
            hp++;
        }
    }
    void healmind(bool cigarettes) {
        if (cigarettes) {
            mind += 10;
        }
    }
    int get_hp() override { return hp; }
    void set_hp(int newhp) { hp = newhp; }
    int get_mind_player() { return mind; }
    int get_loses_count() override { return loses_count; }
    void add_loses_count() override { loses_count++; }
};
class Enemy : public Member {
private:
    int hp;
    int loses_count;

public:

    Enemy() {
        hp = 0;
        loses_count = 0;
    }
    void receive_damage(bool is_hitted) {
        if (is_hitted) {
            hp--;
        }
    }
    void heal(bool energy_drink) {
        if (energy_drink) {
            hp++;
        }
    }
    int get_hp() override { return hp; }
    int get_loses_count() override { return loses_count; }
    void set_hp(int newhp) { hp = newhp; }
    void add_loses_count() override { loses_count++; }
};

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "Russian");
    srand(time(0));
    Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO);
    SDL backscreen(render);
    //backscreen.setRender(render);
    backscreen.loadTexture("pskov_roulette/pov.png");
    backscreen.copyRend();
    LButton btnoff_hit_yourself(render, 200, 100, 1000, 800);
    btnoff_hit_yourself.loadTexture("pskov_roulette/btn_hit_sheet.png", 3);
    btnoff_hit_yourself.copyRend();

    LButton btn_hit_enemy(render, 100, 50, 658, 587);
    btn_hit_enemy.loadTexture("pskov_roulette/btn_hit_sheet.png", 3);
    btn_hit_enemy.copyRend();

    LButton btn_heal(render, 150, 75, 490, 919);
    btn_heal.loadTexture("pskov_roulette/btn_hit_sheet.png", 3);
    btn_heal.copyRend();
    
    TextureText mainText = TextureText();
    mainText.setRenderer(backscreen.getRender());
    mainText.loadFont("pskov_roulette/PixeloidMono.ttf");
   
    SDL_RenderPresent(render);
    Sprite doom_get_shot(backscreen.getRender());
    SDL text();
    ///////////----------------------------------------------КНОПКА ВЫВОДИТСЯ ОСТАЛОСЬ СДЕЛАТЬ ЧТОБЫ ОНА НЕ ПРОПАДАЛА БАЛЯ НЕ ОТРИСОВЫВАЕТСЯ ПО НОВОЙ--------------------------///////хз злой чето был ща вроде норм все
    bool quit = false;
    bool flag_doom_get_shot;
    bool flag_music_doom_get_shot;
    int num_of_energydrinks = 0, num_of_cigarettes = 0; //randomreceiving = 0
    int choosen_bullet = 0;
    bool is_hitted = false;
    Player player;
    Enemy enemy;
    int choice1 = 0, choice2 = 0;
    bool turn_to_shoot = 1; // 1 - очередь игрока, 0 - очередь противника
    //int bullets = 0;
    int amount_of_bullets;
    std::vector<int> gunstore;
    int amount_of_live_bullet_in_gunstore = 0;
    int choice = 0;
    int round = 1;
    //SDL_Event e;
    bool flag = 1; // повторяется ли раунд (0) или переходит к следующему (1)
    std::cout << "Wanna play? press 1" << std::endl;
    //std::cin >> choice1;
    std::string before_shooting;
    std::string gunstore_output;
    while(!quit) 
    {
        while (round < 4)
        { // цикл для того чтобы играть 3 раунда
            turn_to_shoot = 1;

            if (round == 1 && flag) {
                player.set_hp(2);
                enemy.set_hp(2);
            }
            else if (round == 2 && flag) {
                player.set_hp(4);
                enemy.set_hp(4);
            }
            else if (round == 3 && flag) {
                player.set_hp(6);
                enemy.set_hp(6);
            }
            std::cout << "Round " << round << std::endl;
            if (round == 1) {
                amount_of_bullets = 3;
                amount_of_live_bullet_in_gunstore = rand() % (amount_of_bullets - 1) + 1;
            }
            else {
                amount_of_bullets = rand() % (2 * round) + 4;
                amount_of_live_bullet_in_gunstore = rand() % (amount_of_bullets - 1) + 2;
            }
            // amount_of_bullets = rand() % (3 + 2*round); // общее количество пуль в
            // магазине может быть
            //  кол-во боевых зависит от общего кол-ва (всего 3 -> боевых может
            //  быть от 1 до 2)
            for (int i = 0; i < amount_of_bullets; i++) {
                gunstore.push_back(0);
            }
            for (int i = 0; i < amount_of_live_bullet_in_gunstore; i++) {
                gunstore[i] = 1;
            }
            gunstore_output.clear();
            for (int i = 0; i < gunstore.size(); ++i)
            {
                gunstore_output += std::to_string(gunstore[i]) + " ";
            }
            before_shooting.clear();
            before_shooting = "Round " + std::to_string(round) + " player hp: " + std::to_string(player.get_hp()) + " enemy hp: " + std::to_string(enemy.get_hp()) + " gunstore: " + gunstore_output;

            //updateFrame(backscreen, btnoff_hit_yourself, mainText, before_shooting);
            
            backscreen.copyRend();
            btnoff_hit_yourself.copyRend();
            btn_hit_enemy.copyRend();
            btn_heal.copyRend();
            mainText.setText(before_shooting, 400);
            mainText.render(100, 100);
            SDL_RenderPresent(render);
            

            Sleep(5000);
            auto iter = gunstore.begin();
            while (gunstore.size() > 0)
            { // цикл для игры внутри одного раунда
                std::cout << "The gun is loaded" << std::endl;
                // вывод магазина
                iter = gunstore.begin();
                for (int i = 0; i < gunstore.size(); i++) {
                    std::cout << gunstore[i];
                }
                std::cout << std::endl;
                if (turn_to_shoot) 
                {
                    std::cout << "1. Shoot yourself" << std::endl;
                    std::cout << "2. Shoot enemy" << std::endl;
                    std::cout << "3. Heal hp or mind" << std::endl;
                    
                    backscreen.copyRend();
                    btn_hit_enemy.copyRend();
                    btn_heal.copyRend();
                    btnoff_hit_yourself.copyRend();
                    mainText.setText("1. Shoot yourself 2. Shoot enemy 3. Heal hp or mind 0. Exit game", 400);
                    mainText.render(100, 100);
                    SDL_RenderPresent(render);
                    SDL_Event e;
                    do
                    {
                        SDL_WaitEvent(&e);
                        if (e.type == SDL_MOUSEBUTTONDOWN)
                        {
                            int x, y;
                            SDL_GetMouseState(&x, &y);
                            std::cout << x << " " << y << std::endl;
                        }
                        
                        btnoff_hit_yourself.handleEvent(&e);
                        btn_hit_enemy.handleEvent(&e);
                        btn_heal.handleEvent(&e);
                        btnoff_hit_yourself.copyRend();
                        btn_hit_enemy.copyRend();
                        btn_heal.copyRend();
                        SDL_RenderPresent(render);
                    } while (e.type != SDL_MOUSEBUTTONDOWN);
                    
                    if (1)
                    {
                        if (e.type == SDL_QUIT)
                        {
                            quit = true;
                            break;
                        }
                        else if (e.type == SDL_MOUSEBUTTONDOWN)
                        {
                            if(btnoff_hit_yourself.getTrueClicked())
                            {
                                std::cout << "1 button" << std::endl;
                                choosen_bullet = rand() % gunstore.size();
                                is_hitted = gunstore[choosen_bullet];
                                if (gunstore.size() == 1) {
                                    gunstore.clear();
                                }
                                else {
                                    gunstore.erase(iter + choosen_bullet);
                                }
                                player.receive_damage(is_hitted);
                                if (is_hitted) {
                                    flag_doom_get_shot = doom_get_shot.LoadFromFile("pskov_roulette/doomer_split_16frames.png", 16);
                                    flag_music_doom_get_shot = doom_get_shot.loadAudio("pskov_roulette/vyistrelom-iz-drobovika-vyishiblo-mozgi.wav");
                                    animation(doom_get_shot, 16, 13);
                                    doom_get_shot.free();
                                    doom_get_shot.SetRenderer(render);
                                    backscreen.copyRend();
                                    btnoff_hit_yourself.copyRend();
                                    mainText.render(100, 100);
                                    SDL_RenderPresent(render);
                                    SDL_RenderPresent(render);
                                    Sleep(3000);
                                    /*
                                    std::cout << "Ты ебанутый там боевой патрон" << std::endl;
                                    std::cout << "GOAAAAAAAAAAAAAAAAAAL" << std::endl;
                                    std::cout << "Your current hp " << player.get_hp() << std::endl;
                                    */
                                    
                                    turn_to_shoot = 0;
                                }
                                else {
                                    //std::cout << "Повезло? там холостой" << std::endl;
                                    
                                    backscreen.copyRend();
                                    btnoff_hit_yourself.copyRend();
                                    mainText.setText("Lucky? It was blank one", 400);
                                    mainText.render(100, 100);
                                    SDL_RenderPresent(render);
                                    SDL_RenderPresent(render);
                                    Sleep(3000);
                                    turn_to_shoot = 1;
                                }
                                if (player.get_hp() == 0) {
                                    //std::cout << "You died" << std::endl;
                                    
                                    backscreen.copyRend();
                                    btnoff_hit_yourself.copyRend();
                                    mainText.setText("You died", 400);
                                    mainText.render(100, 100);
                                    SDL_RenderPresent(render);
                                    Sleep(3000);
                                    player.add_loses_count();
                                    flag = 1;
                                    turn_to_shoot = 1;
                                    break;
                                }
                                btnoff_hit_yourself.ZeroTrueClicked();
                            }
                            else if (btn_hit_enemy.getTrueClicked())
                            {
                                std::cout << "2 button" << std::endl;
                                choosen_bullet = rand() % gunstore.size();
                                is_hitted = gunstore[choosen_bullet];
                                if (gunstore.size() == 1) {
                                    gunstore.clear();
                                }
                                else {
                                    gunstore.erase(iter + choosen_bullet);
                                }
                                enemy.receive_damage(is_hitted);
                                if (is_hitted) {
                                    //std::cout << "You hitted enemy" << std::endl;
                                    //std::cout << "Enemys current hp " << enemy.get_hp() << std::endl;
                                    before_shooting.clear();
                                    before_shooting = "You hitted enemy. Enemys current hp: " + std::to_string(enemy.get_hp());
                                    if (rand() % 2 == 0) {
                                        //std::cout << "You received 1 energy drink" << std::endl;
                                        before_shooting += "You received 1 energy drink";
                                        num_of_energydrinks++;
                                    }
                                    else {
                                        //std::cout << "You received 1 cigarette" << std::endl;
                                        before_shooting += "You received 1 cigarette";
                                        num_of_cigarettes++;
                                    }

                                    backscreen.copyRend();
                                    btnoff_hit_yourself.copyRend();
                                    mainText.setText(before_shooting, 400);
                                    mainText.render(100, 100);
                                    SDL_RenderPresent(render);

                                    Sleep(5000);
                                    if (enemy.get_hp() == 0) {
                                        //std::cout << "Enemy loses" << std::endl;

                                        backscreen.copyRend();
                                        btnoff_hit_yourself.copyRend();
                                        mainText.setText("Enemy loses", 400);
                                        mainText.render(100, 100);
                                        SDL_RenderPresent(render);

                                        //updateFrame(backscreen, btnoff_hit_yourself, mainText, "Enemy loses");
                                        enemy.add_loses_count();
                                        flag = 1;
                                        Sleep(5000);
                                        break;
                                    }
                                }
                                else {
                                    //std::cout << "You missed" << std::endl;

                                    backscreen.copyRend();
                                    mainText.setText("You missed", 400);
                                    mainText.render(100, 100);
                                    SDL_RenderPresent(backscreen.getRender());

                                    //updateFrame(backscreen, btnoff_hit_yourself, mainText, "You missed");
                                    Sleep(5000);
                                }
                                turn_to_shoot = 0;
                                btn_hit_enemy.ZeroTrueClicked();
                            }
                            else if (btn_heal.getTrueClicked())
                            {
                                std::cout << "3 button" << std::endl;
                                if (num_of_cigarettes > 0 || num_of_energydrinks > 0)
                                {
                                    std::cout << "You have " << num_of_cigarettes << " cigarettes "
                                        << "and " << num_of_energydrinks << " energy drinks"
                                        << std::endl;
                                    std::cout << "What do you want to heal? 1 - hp (drink energy "
                                        "drink), 2 - mind (smoke cigarette)"
                                        << std::endl;
                                    std::cin >> choice2;
                                }
                                else
                                {
                                    std::cout << "You don`t have enough cigarettes or energy drinks to heal" << std::endl;
                                }
                                if (choice2 == 1) {
                                    if (num_of_energydrinks > 0) {
                                        player.healhp(true);
                                        num_of_energydrinks--;
                                        std::cout << "You healed yourself" << std::endl;
                                        std::cout << "You have " << num_of_energydrinks
                                            << " energy drinks left" << std::endl;
                                        std::cout << "You have " << player.get_hp() << " hp left"
                                            << std::endl;
                                    }
                                    else
                                        std::cout << "You don't have energy drinks" << std::endl;
                                }
                                else {
                                    if (num_of_cigarettes > 0) {
                                        player.healmind(true);
                                        num_of_cigarettes--;
                                        std::cout << "You healed yourself" << std::endl;
                                        std::cout << "You have " << num_of_cigarettes
                                            << " cigarettes left" << std::endl;
                                        std::cout << "You have " << player.get_mind_player()
                                            << " mind left" << std::endl;
                                    }
                                    else
                                        std::cout << "You don't have cigarettes" << std::endl;
                                }
                                btn_heal.ZeroTrueClicked();
                            }
                            else if(0){ // тут надо добавить кнопку выхода
                                std::cout << "0 button" << std::endl;
                                std::cout << "Game over" << std::endl;
                                return 0;
                            }
                        }
                    }
                }
                if (gunstore.size() > 0 && turn_to_shoot == 0) {
                    std::cout << "Enemy shoots" << std::endl;
                    iter = gunstore.begin();
                    choosen_bullet = rand() % gunstore.size();
                    is_hitted = gunstore[choosen_bullet];

                    amount_of_bullets = gunstore.size();
                    float count_live_bullets = 0;
                    for (int i = 0; i < gunstore.size(); ++i) {
                        if (gunstore[i] == 1)
                            count_live_bullets += 1;
                    }
                    if (gunstore.size() == 1) {
                        gunstore.clear();
                    }
                    else {
                        gunstore.erase(iter + choosen_bullet);
                    }
                    if (count_live_bullets / amount_of_bullets >= 0.5) {
                        player.receive_damage(is_hitted);
                        if (is_hitted) {
                            //std::cout << "Ты получил в ебало" << std::endl;
                            //std::cout << "Your current hp " << player.get_hp() << std::endl;
                            backscreen.copyRend();
                            mainText.setText("You got damage", 400);
                            mainText.render(100, 100);
                            SDL_RenderPresent(backscreen.getRender());
                            Sleep(2000);
                            if (player.get_hp() == 0) {
                                //std::cout << "You died" << std::endl;
                                backscreen.copyRend();
                                mainText.setText("You died", 400);
                                mainText.render(100, 100);
                                SDL_RenderPresent(backscreen.getRender());
                                Sleep(2000);
                                player.add_loses_count();
                                flag = 1;
                                break;
                            }
                        }
                        else {
                            //std::cout << "Player: Повезло? там холостой" << std::endl;
                            backscreen.copyRend();
                            mainText.setText("Player: I am lucky", 400);
                            mainText.render(100, 100);
                            SDL_RenderPresent(backscreen.getRender());
                            Sleep(2000);
                        }
                        turn_to_shoot = 1;
                    }
                    else {
                        enemy.receive_damage(is_hitted);
                        if (is_hitted) {
                            //std::cout << "Enemy: Блять я себя захуярил" << std::endl;
                            //std::cout << "Enemys current hp " << enemy.get_hp() << std::endl;
                            backscreen.copyRend();
                            mainText.setText("Enemy: fuck, I hit myself", 400);
                            mainText.render(100, 100);
                            SDL_RenderPresent(backscreen.getRender());
                            Sleep(2000);
                            turn_to_shoot = 1;
                        }
                        else {
                            //std::cout << "Enemy: Ахахахахахахахах повезло холостой" << std::endl;
                            turn_to_shoot = 0;
                        }
                        if (enemy.get_hp() == 0) {
                            //std::cout << "Enemy loses" << std::endl;
                            backscreen.copyRend();
                            mainText.setText("Enemy: AHAHAHHAHAH lucky there is a blank one", 400);
                            mainText.render(100, 100);
                            SDL_RenderPresent(backscreen.getRender());
                            Sleep(2000);
                            enemy.add_loses_count();
                            flag = 1;
                            break;
                        }
                    }
                }
                if (enemy.get_hp() == 0 || player.get_hp() == 0)
                {
                    break;
                }
                
            }
            round++;
            if (player.get_hp() > 0 && enemy.get_hp() > 0) {
                round--;
                flag = 0;
                //std::cout << "You are out of bullets, but still alive" << std::endl;
                //std::cout << "Round continues" << std::endl;
                backscreen.copyRend();
                mainText.setText("You are out of bullets, but still alive. Round continues", 400);
                mainText.render(100, 100);
                SDL_RenderPresent(backscreen.getRender());
                Sleep(3000);
            }
            else {
                backscreen.copyRend();
                mainText.setText("You are out of bullets. Round ends", 400);
                mainText.render(100, 100);
                SDL_RenderPresent(backscreen.getRender());
                Sleep(3000);
            }
            
        }
        //std::cout << "You have " << player.get_loses_count() << " loses" << std::endl;
        //std::cout << "Enemy has " << enemy.get_loses_count() << " loses" << std::endl;
        backscreen.copyRend();
        mainText.setText("You have " + std::to_string(player.get_loses_count()) + " loses. Enemy has " + std::to_string(enemy.get_loses_count()) + " loses.", 600);
        mainText.render(100, 100);
        SDL_RenderPresent(backscreen.getRender());
        Sleep(5000);
        break;
    }
    return 0;
}



