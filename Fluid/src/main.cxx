#include "pch.hxx"

constexpr uint16_t gridWidth = 120;
constexpr uint16_t gridHeight = 60;

constexpr uint8_t iterations = 40;

constexpr double cellSize = 1.0;
constexpr double timeStamp = 1.0 / 60.0;
constexpr double density = 1.0;

constexpr glm::dvec2 boundSize = glm::dvec2(gridWidth, gridHeight) * cellSize;
constexpr glm::dvec2 bottomLeft = -boundSize / 2.0;
constexpr double halfCellSize = cellSize / 2.0;

std::array<std::array<double, gridHeight + 1>, gridWidth> velocityRows;
std::array<std::array<double, gridHeight>, gridWidth + 1> velocityColumns;
std::array<std::array<double, gridHeight>, gridWidth> pressures;
std::array<std::array<uint8_t, gridHeight>, gridWidth> isFluid;

double Divergance(uint16_t x, uint16_t y) {
    double velocityLeft = velocityColumns[x][y];
    double velocityRight = velocityColumns[x + 1][y];
    double velocityBottom = velocityRows[x][y];
    double velocityTop = velocityRows[x][y + 1];

    return (velocityRight - velocityLeft + velocityTop - velocityBottom) / cellSize;
}

double GetPressure(uint16_t x, uint16_t y) {
    bool outOfBounds = x < 0 or x >= gridWidth or y < 0 or y >= gridHeight;
    return outOfBounds ? 0 : pressures[x][y];
}

uint8_t IsFluid(uint16_t x, uint16_t y) {
    bool outOfBounds = x < 0 or x >= gridWidth or y < 0 or y >= gridHeight;
    return outOfBounds ? 1 : isFluid[x][y];
}

uint8_t IsSolid(uint16_t x, uint16_t y) {
    return not IsFluid(x, y);
}

void UpdatePressures() {
    for (uint16_t x = 0; x < gridWidth; x++) {
        for (uint16_t y = 0; y < gridHeight; y++) {
            if (IsSolid(x, y)) {
                pressures[x][y] = 0.0;
                continue;
            }

            uint8_t fluidLeft = IsFluid(x - 1, y);
            uint8_t fluidRight = IsFluid(x + 1, y);
            uint8_t fluidBottom = IsFluid(x, y - 1);
            uint8_t fluidTop = IsFluid(x, y + 1);
            uint8_t fluidCells = fluidLeft + fluidRight + fluidBottom + fluidTop;

            if (fluidCells == 0) {
                pressures[x][y] = 0.0;
                continue;
            }

            double pressureLeft = GetPressure(x - 1, y) * fluidLeft;
            double pressureRight = GetPressure(x + 1, y) * fluidRight;
            double pressureBottom = GetPressure(x, y - 1) * fluidBottom;
            double pressureTop = GetPressure(x, y + 1) * fluidTop;

            double pressureSum = pressureLeft + pressureRight + pressureBottom + pressureTop;

            double velocityLeft = velocityColumns[x][y] * fluidLeft;
            double velocityRight = velocityColumns[x + 1][y] * fluidRight;
            double velocityBottom = velocityRows[x][y] * fluidBottom;
            double velocityTop = velocityRows[x][y + 1] * fluidTop;

            double velocitySum = velocityRight - velocityLeft + velocityTop - velocityBottom;

            double pressure = (pressureSum - density * cellSize * velocitySum / timeStamp) / fluidCells;
            double oldPressure = pressures[x][y];

            pressures[x][y] = oldPressure + (pressure - oldPressure) * 1.7;
        }
    }
}

void UpdateVelocities() {
    constexpr double k = timeStamp / (density * cellSize);

    for (uint16_t x = 0; x < gridWidth; x++) {
        for (uint16_t y = 0; y < gridHeight + 1; y++) {
            if (IsSolid(x, y - 1) or IsSolid(x, y)) {
                velocityRows[x][y] = 0.0;
                continue;
            }

            double pressureBottom = GetPressure(x, y - 1);
            double pressureTop = GetPressure(x, y);
            velocityRows[x][y] -= k * (pressureTop - pressureBottom);
        }
    }

    for (uint16_t x = 0; x < gridWidth + 1; x++) {
        for (uint16_t y = 0; y < gridHeight; y++) {
            if (IsSolid(x - 1, y) or IsSolid(x, y)) {
                velocityColumns[x][y] = 0.0;
                continue;
            }

            double pressureLeft = GetPressure(x - 1, y);
            double pressureRight = GetPressure(x, y);
            velocityColumns[x][y] -= k * (pressureRight - pressureLeft);
        }
    }
}

template<typename T>
T Clamp(T x, T min, T max) {
    if (x < min) {
        return min;
    }

    if (x > max) {
        return max;
    }

    return x;
}

template<typename T>
T Lerp(T t, T A, T B) {
    return A + (B - A) * t;
}

glm::dvec2 GetVelocity(double x, double y) {
    double vx;
    {
        double width = (gridWidth - 1) * cellSize;
        double height = gridHeight * cellSize;
    
        double px = (x + width / 2.0) / cellSize;
        double py = (y + height / 2.0) / cellSize;
    
        uint16_t left = Clamp<uint16_t>((uint16_t)px, 0, gridWidth - 2);
        uint16_t right = left + 1;
        uint16_t bottom = Clamp<uint16_t>((uint16_t)py, 0, gridHeight - 1);
        uint16_t top = bottom + 1;
    
        double xFrac = Clamp<double>(px - left, 0.0, 1.0);
        double yFrac = Clamp<double>(py - bottom, 0.0, 1.0);
    
        double valueTop = Lerp<double>(xFrac, velocityRows[left][top], velocityRows[right][top]);
        double valueBottom = Lerp<double>(xFrac, velocityRows[left][bottom], velocityRows[right][bottom]);
        vx = Lerp<double>(yFrac, valueBottom, valueTop);
    }

    double vy;
    {
        double width = gridWidth * cellSize;
        double height = (gridHeight - 1) * cellSize;
    
        double px = (x + width / 2.0) / cellSize;
        double py = (y + height / 2.0) / cellSize;
    
        uint16_t left = Clamp<uint16_t>((uint16_t)px, 0, gridWidth - 1);
        uint16_t right = left + 1;
        uint16_t bottom = Clamp<uint16_t>((uint16_t)py, 0, gridHeight - 2);
        uint16_t top = bottom + 1;
    
        double xFrac = Clamp<double>(px - left, 0.0, 1.0);
        double yFrac = Clamp<double>(py - bottom, 0.0, 1.0);
    
        double valueTop = Lerp<double>(xFrac, velocityColumns[left][top], velocityColumns[right][top]);
        double valueBottom = Lerp<double>(xFrac, velocityColumns[left][bottom], velocityColumns[right][bottom]);
        vy = Lerp<double>(yFrac, valueBottom, valueTop);
    }

    return { vx, vy };
}

glm::dvec2 CellCentre(uint16_t x, uint16_t y) {
    return bottomLeft + glm::dvec2(x + 0.5, y + 0.5) * cellSize;
}

glm::dvec2 LeftEdgeCentre(uint16_t x, uint16_t y) {
    return CellCentre(x, y) - glm::dvec2(halfCellSize, 0.0);
}

glm::dvec2 BottomEdgeCentre(uint16_t x, uint16_t y) {
    return CellCentre(x, y) - glm::dvec2(0.0, halfCellSize);
}

void AdvectVelocity() {
    static std::array<std::array<double, gridHeight + 1>, gridWidth> newVelocityRows;
    for (uint16_t x = 0; x < gridWidth; x++) {
        for (uint16_t y = 0; y < gridHeight + 1; y++) {
            if (IsSolid(x, y - 1) or IsSolid(x, y)) {
                continue;
            }
            
            glm::dvec2 position = LeftEdgeCentre(x, y);
            glm::dvec2 velocity = GetVelocity(position.x, position.y);
            glm::dvec2 previousPosition = position - velocity * timeStamp;
            newVelocityRows[x][y] = GetVelocity(previousPosition.x, previousPosition.y).x;
        }
    }
    
    static std::array<std::array<double, gridHeight>, gridWidth + 1> newVelocityColumns;
    for (uint16_t x = 0; x < gridWidth + 1; x++) {
        for (uint16_t y = 0; y < gridHeight; y++) {
            if (IsSolid(x - 1, y) or IsSolid(x, y)) {
                continue;
            }

            glm::dvec2 position = LeftEdgeCentre(x, y);
            glm::dvec2 velocity = GetVelocity(position.x, position.y);
            glm::dvec2 previousPosition = position - velocity * timeStamp;
            newVelocityColumns[x][y] = GetVelocity(previousPosition.x, previousPosition.y).y;
        }
    }

    for (uint16_t x = 0; x < gridWidth; x++) {
        velocityRows[x] = newVelocityRows[x];
    }

    for (uint16_t x = 0; x < gridWidth + 1; x++) {
        velocityColumns[x] = newVelocityColumns[x];
    }
}

int main() {
    auto window = Automata::Window(1200, 600, "Fluid");

    auto shader = Automata::Shader({{GL_VERTEX_SHADER, SOURCE_DIRECTORY "/Fluid/shaders/vertex.vert"},
                                    {GL_FRAGMENT_SHADER, SOURCE_DIRECTORY "/Fluid/shaders/fragment.frag"}});

    shader.Bind();

    glm::mat4 projection =
        glm::ortho(0.0f, static_cast<float>(window.GetWidth()), static_cast<float>(window.GetHeight()), 0.0f, -1.0f, 1.0f);
    shader.SetMatrix4fv("projection", projection);

    auto vertexArray = Automata::VertexArray();
    vertexArray.Bind();

    std::array<GLfloat, 8> vertices = {0.0f, 0.0f, 0.0f, 10.0f, 10.0f, 0.0f, 10.0f, 10.0f};

    auto vertexBuffer = Automata::VertexBuffer(vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
    vertexBuffer.Bind();

    auto bufferLayout = Automata::BufferLayout({{2, GL_FLOAT}});

    bufferLayout.Enable();

    std::array<GLuint, 6> indicies = {0, 1, 2, 1, 2, 3};

    auto elementBuffer =
        Automata::ElementBuffer(indicies.size() * sizeof(GLuint), indicies.data(), GL_STATIC_DRAW, GL_UNSIGNED_INT);
    elementBuffer.Bind();

    for (auto &column : pressures) {
        column.fill(0.0);
    }

    for (auto &column : isFluid) {
        column.fill(1);
    }

    isFluid[0].fill(0);

    for (auto &column : isFluid) {
        column[0] = column[gridHeight - 1] = 0;
    }

    for (uint16_t y = 25; y < 35; y++) {
        isFluid[0][y] = 1;
    }

    for (uint16_t x = 15; x < 45; x++) {
        for (uint16_t y = 20; y < 40; y++) {
            if ((x - 29.5) * (x - 29.5) + (y - 29.5) * (y - 29.5) <= 10 * 10) {
                isFluid[x][y] = 0;
            }
        }
    }

    while (true) {
        glClear(GL_COLOR_BUFFER_BIT);

        for (uint8_t iteration = 0; iteration < iterations; iteration++) {
            UpdatePressures();
        }

        UpdateVelocities();

        velocityColumns[0].fill(9.81);

        for (uint16_t x = 0; x < gridWidth; x++) {
            for (uint16_t y = 0; y < gridHeight; y++) {
                double pressure = GetPressure(x, y);
                double pressureT = std::abs(pressure) / 1.4;

                float red = Lerp<double>(0.122641504, pressure < 0.0 ? 1.0 : 0.33490568, pressureT);
                float green = Lerp<double>(0.122641504, pressure < 0.0 ? 0.3254717 : 0.6240772, pressureT);
                float blue = Lerp<double>(0.122641504, pressure < 0.0 ? 0.3254717 : 1.0, pressureT);
                shader.Set4f("color", red, green, blue, 1.0f);

                glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(x * (static_cast<float>(window.GetWidth()) / gridWidth), y * (static_cast<float>(window.GetHeight()) / gridHeight), 0.0f));
                shader.SetMatrix4fv("model", model);

                glDrawElements(GL_TRIANGLES, elementBuffer.GetCount(), elementBuffer.GetType(), 0);
            }
        }

        // AdvectSmoke();
        AdvectVelocity();

        window.Update();
    }

    return 0;
}
