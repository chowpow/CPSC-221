#include "block.h"

void Block::Build(PNG& im, int upper, int left, int dimension) {
    for (int y = upper; y < (upper + dimension); y++) {
        vector<HSLAPixel> innerData;
        for (int x = left; x < (left + dimension); x++) {
            HSLAPixel* pixel = im.getPixel(x,y);
            innerData.push_back(*pixel);
        }
        data.push_back(innerData);
    }

}

void Block::Render(PNG& im, int upper, int left) const {
    int dimension = Dimension();
    for (int y = upper; y < upper + dimension; y++) {
      for (int x = left; x < left + dimension; x++) {
        *im.getPixel(x,y) = data[y-upper][x-left]; 
      }
    }
}

void Block::Negative() {
    for (unsigned int i = 0; i<data.size(); i++) {
        for (unsigned int j = 0; j<data[i].size(); j++) {
            if (data[i][j].h < 180) {
                data[i][j].h += 180;
            } else {
                data[i][j].h -= 180;    
            }

            if (data[i][j].l < 0.5) {
                data[i][j].l = 2 * (0.5 - data[i][j].l) + data[i][j].l;
            } else {
                data[i][j].l = data[i][j].l - (2 * (data[i][j].l - 0.5));
            }


        }
    }
}

int Block::Dimension() const {
    return data.size();
}

