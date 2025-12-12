#include "Physics.h"
#include <random>

double dot(const Point& lhs, const Point& rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y;
}

Physics::Physics(double timePerTick) : timePerTick{timePerTick} {}

void Physics::setWorldBox(const Point& topLeft, const Point& bottomRight) {
    this->topLeft = topLeft;
    this->bottomRight = bottomRight;
}

void Physics::update(std::vector<Ball>& balls, std::vector<Dust>& dusts, const size_t ticks) const {

    for (size_t i = 0; i < ticks; ++i) {
        move(balls);
        collideWithBox(balls);
        collideBalls(balls, dusts);
    }
}

void Physics::collideBalls(std::vector<Ball>& balls, std::vector<Dust>& dusts) const {
    for (auto a = balls.begin(); a != balls.end(); ++a) {
        for (auto b = std::next(a); b != balls.end(); ++b) {
            if(!a->getCollidable() || !b->getCollidable()) continue;
            const double distanceBetweenCenters2 =
                distance2(a->getCenter(), b->getCenter());
            const double collisionDistance = a->getRadius() + b->getRadius();
            const double collisionDistance2 =
                collisionDistance * collisionDistance;

            if (distanceBetweenCenters2 < collisionDistance2) {
                processCollision(*a, *b, distanceBetweenCenters2, dusts);
            }
        }
    }
}

void Physics::collideWithBox(std::vector<Ball>& balls) const {
    for (Ball& ball : balls) {
        if(!ball.getCollidable()) continue;
        const Point p = ball.getCenter();
        const double r = ball.getRadius();
        // определяет, находится ли v в диапазоне (lo, hi) (не включая границы)
        auto isOutOfRange = [](double v, double lo, double hi) {
            return v < lo || v > hi;
        };

        if (isOutOfRange(p.x, topLeft.x + r, bottomRight.x - r)) {
            Point vector = ball.getVelocity().vector();
            vector.x = -vector.x;
            ball.setVelocity(vector);
        } else if (isOutOfRange(p.y, topLeft.y + r, bottomRight.y - r)) {
            Point vector = ball.getVelocity().vector();
            vector.y = -vector.y;
            ball.setVelocity(vector);
        }
    }
}

void Physics::move(std::vector<Ball>& balls) const {
    for (Ball& ball : balls) {
        Point newPos =
            ball.getCenter() + ball.getVelocity().vector() * timePerTick;
        ball.setCenter(newPos);
    }
}

void Physics::processCollision(Ball& a, Ball& b,
                               double distanceBetweenCenters2, std::vector<Dust>& dusts) const {
    // нормированный вектор столкновения
    const Point normal = (b.getCenter() - a.getCenter()) / std::sqrt(distanceBetweenCenters2);

    // получаем скорость в векторном виде
    const Point aV = a.getVelocity().vector();
    const Point bV = b.getVelocity().vector();

    // коэффициент p учитывает скорость обоих мячей
    const double p = 2 * (dot(aV, normal) - dot(bV, normal)) / (a.getMass() + b.getMass());

    // задаем новые скорости мячей после столкновения
    a.setVelocity(Velocity(aV - normal * p * a.getMass()));
    b.setVelocity(Velocity(bV + normal * p * b.getMass()));

    // --- Генерация частиц ---
    int n = 10; // Количество частиц
    double radius = 2.5; // Радиус частицы
    double lifetime = 2.0; // Время жизни частицы (в секундах)
    Color dustColor(0, 0, 0); // Серый цвет, можно сделать смесь цветов шаров

    // Точка столкновения (приблизительно)
    Point collisionPoint = (a.getCenter() + b.getCenter()) / 2.0;

    // Инициализация генератора случайных чисел
    static std::random_device rd;
    static std::mt19937 gen(rd());
    // Диапазон для случайной скорости частицы (абсолютное значение)
    static std::uniform_real_distribution<> speedDist(80.0, 100.0); // Пример диапазона
    // Диапазон для случайного угла
    static std::uniform_real_distribution<> angleDist(0.0, 2 * M_PI); // От 0 до 2*pi

    for (int i = 0; i < n; ++i) {
        double speed = speedDist(gen);
        double angle = angleDist(gen);
        Velocity dustVelocity(speed, angle);
        Dust newDust(collisionPoint, dustVelocity, radius, dustColor, lifetime);
        dusts.push_back(newDust);
    }}
