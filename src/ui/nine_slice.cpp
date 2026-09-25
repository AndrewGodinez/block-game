#include "nine_slice.h"

NineSlice::NineSlice()
    : textureLoaded(false), marginLeft(35.f), marginTop(35.f), marginRight(35.f), marginBottom(35.f) {}

bool NineSlice::loadFromFile(const std::string& filepath, float ml, float mt, float mr, float mb) {
    marginLeft = ml;
    marginTop = mt;
    marginRight = mr;
    marginBottom = mb;
    textureLoaded = texture.loadFromFile(filepath) || texture.loadFromFile("../" + filepath);
    return textureLoaded;
}

bool NineSlice::isLoaded() const {
    return textureLoaded;
}

void NineSlice::draw(sf::RenderTarget& target, sf::FloatRect bounds, sf::Color tint) const {
    if (!textureLoaded) return;

    float tw = static_cast<float>(texture.getSize().x);
    float th = static_cast<float>(texture.getSize().y);

    float ml = marginLeft;
    float mt = marginTop;
    float mr = marginRight;
    float mb = marginBottom;

    float x0 = bounds.position.x;
    float x1 = x0 + ml;
    float x2 = x0 + bounds.size.x - mr;
    float x3 = x0 + bounds.size.x;

    float y0 = bounds.position.y;
    float y1 = y0 + mt;
    float y2 = y0 + bounds.size.y - mb;
    float y3 = y0 + bounds.size.y;

    float u0 = 0.f, u1 = ml, u2 = tw - mr, u3 = tw;
    float v0 = 0.f, v1 = mt, v2 = th - mb, v3 = th;

    float xs[4] = {x0, x1, x2, x3};
    float ys[4] = {y0, y1, y2, y3};
    float us[4] = {u0, u1, u2, u3};
    float vs[4] = {v0, v1, v2, v3};

    sf::VertexArray va(sf::PrimitiveType::Triangles, 54);
    int idx = 0;

    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            float rx0 = xs[col], rx1 = xs[col + 1];
            float ry0 = ys[row], ry1 = ys[row + 1];
            float ru0 = us[col], ru1 = us[col + 1];
            float rv0 = vs[row], rv1 = vs[row + 1];

            va[idx + 0] = sf::Vertex{{rx0, ry0}, tint, {ru0, rv0}};
            va[idx + 1] = sf::Vertex{{rx1, ry0}, tint, {ru1, rv0}};
            va[idx + 2] = sf::Vertex{{rx0, ry1}, tint, {ru0, rv1}};

            va[idx + 3] = sf::Vertex{{rx1, ry0}, tint, {ru1, rv0}};
            va[idx + 4] = sf::Vertex{{rx1, ry1}, tint, {ru1, rv1}};
            va[idx + 5] = sf::Vertex{{rx0, ry1}, tint, {ru0, rv1}};

            idx += 6;
        }
    }

    sf::RenderStates states(&texture);
    target.draw(va, states);
}
