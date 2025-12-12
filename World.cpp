#include "World.h"
#include "Painter.h"
#include <fstream>
#include <algorithm>
// Длительность одного тика симуляции.
// Подробнее см. update()
// Изменять не следует
static constexpr double timePerTick = 0.001;

/**
 * Конструирует объект мира для симуляции
 * @param worldFilePath путь к файлу модели мира
 */
World::World(const std::string& worldFilePath) {

    std::ifstream stream(worldFilePath);
    /**
     * TODO: хорошее место для улучшения.
     * Чтение границ мира из модели
     * Обратите внимание, что здесь и далее мы многократно
     * читаем в объект типа Point, последовательно
     * заполняя координаты x и у. Если что-то делаем
     * многократно - хорошо бы вынести это в функцию
     * и не дублировать код...
     */
    stream >>topLeft>>bottomRight;
    physics.setWorldBox(topLeft, bottomRight);

    /**
     * TODO: хорошее место для улучшения.
     * (x, y) и (vx, vy) - составные части объекта, также
     * как и (red, green, blue). Опять же, можно упростить
     * этот код, научившись читать сразу Point, Color...
     */
    Point center;
    Color color;
    Velocity velocity;
    double radius;
    bool isCollidable;

    // Здесь не хватает обработки ошибок, но на текущем
    // уровне прохождения курса нас это устраивает
    while (stream>>center>>velocity>>color>>radius>>std::boolalpha>>isCollidable) {
        // Читаем координаты центра шара (x, y) и вектор
        // его скорости (vx, vy)
        // Читаем три составляющие цвета шара
        // Читаем радиус шара
        // Читаем свойство шара isCollidable, которое
        // указывает, требуется ли обрабатывать пересечение
        // шаров как столкновение. Если true - требуется.
        // В базовой части задания этот параметр
        Ball ball =Ball(center,velocity,isCollidable,radius,color);
        balls.push_back(ball);
        // TODO: место для доработки.
        // Здесь не хватает самого главного - создания
        // объекта класса Ball со свойствами, прочитанными
        // выше, и его помещения в контейнер balls

        // После того как мы каким-то образом
        // сконструируем объект Ball ball;
        // добавьте его в конец контейнера вызовом
        // balls.push_back(ball);
    }
}

/// @brief Отображает состояние мира
void World::show(Painter& painter) const {
    // Рисуем белый прямоугольник, отображающий границу
    // мира
    painter.draw(topLeft, bottomRight, Color(1, 1, 1));

    // Вызываем отрисовку каждого шара
    for (const Ball& ball : balls) {
        ball.draw(painter);
    }
    for (const Dust& dust:dusts){
        dust.draw(painter);
    }
}

/// @brief Обновляет состояние мира
void World::update(double time) {
    // учитываем остаток времени, который мы не "доработали" при прошлом update
    time += restTime;
    const auto ticks = static_cast<size_t>(std::floor(time / timePerTick));
    restTime = time - double(ticks) * timePerTick;

    // Обновляем шары и частицы через физический движок
    // Передаем вектор частиц по ссылке
    physics.update(balls, dusts, ticks);

    // Обновляем таймеры частиц
    for (auto& dust : dusts) {
        dust.update(timePerTick); // Обновляем на длительность одного тика
    }

    // Удаляем "мертвые" частицы
    // std::remove_if перемещает "мертвые" элементы в конец вектора
    dusts.erase(
        std::remove_if(dusts.begin(), dusts.end(),
                       [](const Dust& d) { return d.rDead(); }),
        dusts.end()
    );
}
