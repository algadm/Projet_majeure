#include <GL/glew.h>

#include "river.hpp"

#include "../interface/myWidgetGL.hpp"
#include "../../lib/mesh/mesh_io.hpp"

#include <cmath>
#include <string>
#include <sstream>
#include <limits>
#include <array>
#include <queue>
#include <utility>
#include <iostream>
#include <vector>

using namespace cpe;

river::river(int const n, int const m, float t, cpe::mesh_parametric s)
{
    N = n;
    M = m;
    thresh = t;
    water.resize(n * m, t);
    labels.resize(n * m, -1);
    data.resize(n * m, 0);
    for (int i = 0; i < m; i++){
        for (int j = 0; j < n; j++) {
            if (s.vertex(i,j).z() <= t) {
                data[i*m+j] = 0;
            }
        }
    }
}

void river::connected_components(int min_size)
{
    int label = 0;
    std::vector<int> dx = {1, -1, 0, 0};
    std::vector<int> dy = {0, 0, 1, -1};
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            if (data[i*M+j] != 0 && labels[i*M+j] == -1) {
                queue<pair<int, int>> q;
                q.push({i, j});
                labels[i*M+j] = label;
                int component_size = 0;

                while (!q.empty()) {
                    auto [cx, cy] = q.front();
                    q.pop();
                    component_size += 1;

                    for (int d = 0; d < 4; ++d) {
                        int nx = cx + dx[d];
                        int ny = cy + dy[d];
                        if (nx >= 0 && nx < M && ny >=0 && ny < N && data[nx*M+ny] != 0 && labels[nx*M+ny] == -1) {
                            q.push({nx, ny});
                            labels[nx*M+ny] = label;
                        }
                    }
                }

                if (component_size < min_size) {
                    for (int i = 0; i < labels.size(); i++) {
                        if (labels[i] == label) {
                            labels[i] = 0;
                        }
                    }
                }
                label += 1;
            }
        }
    }

}

void river::update_water(int min_size) {
    connected_components(min_size);
    for (int i = 0; i < M*N; i++) {
        if (labels[i] == -1) {
            water[i] = thresh - 10.0f;
        }
    }
}

std::vector<float> river::get_water() {
    return water;
}