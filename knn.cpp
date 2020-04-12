#include <iostream>
#include <vector>
#include <ctime>
#include <chrono>
#include <cstdio>
#include <unistd.h>
#include <cmath>
using namespace std;

template <typename T>
class CartesianCoord {
  public:
    typedef T value_t;
    value_t x;
    value_t y;

    CartesianCoord(value_t _x = 0, value_t _y = 0): x(_x), y(_y) {
      
    }

    ~CartesianCoord(void) {
    
    }

    template <typename _T>
    friend ostream& operator<<(ostream &out, const CartesianCoord<_T> c) {
      out << "(" << c.x << "," << c.y << ")";
      return out;
    }

};

typedef CartesianCoord<int> coord_t;

double euclideanDistance(coord_t &a, const coord_t &b) {
  return sqrt((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y));
}

void maxHeapify(vector<pair<coord_t*, double>> &v, int size, int i) {
  int max = i;
  int left = 2*i + 1;
  int right = 2*i + 2;

  if (left < size && v[left].second > v[max].second)
    max = left;

  if (right < size && v[right].second > v[max].second)
    max = right;

  if (max != i) {
    swap(v[i], v[max]);
    maxHeapify(v, size, max);
  }
}

void heapSort(vector<pair<coord_t*, double>> &v) {
  for (int i = v.size() / 2 - 1; i >= 0; i--)
    maxHeapify(v, v.size(), i);

  for (int i = v.size() - 1; i >= 0; i--) {
    swap(v[0], v[i]);
    maxHeapify(v, i, 0);
  }
}

vector<coord_t> knn (int k, vector<coord_t> &points, const coord_t &q) {
  vector<coord_t> result(k);
  vector<pair<coord_t*, double>> dist;

  for (auto &i : points) {
    int distance = euclideanDistance(i, q);
    dist.push_back(make_pair(&i, distance)); 
  }

  heapSort(dist);
  
  for (int i = 0; i < result.size(); ++i)
    result[i] = *dist[i].first;

  return result;
}

int main()
{
  srand (time(NULL));

  vector<coord_t> points;
  for (int i = 0; i < 1000; i++)
    points.push_back(coord_t(rand()%1000, rand()%1000));

  vector<coord_t>::iterator it = points.begin();
  for (; it != points.end(); it++) {
    fflush(stdout);
    cout << "\r" << *it;
    usleep(2000);
  }

  cout << endl;

  vector<coord_t> knns;
  auto start = chrono::high_resolution_clock::now();
  knns = knn(3, points, coord_t(100, 200));
  auto stop = chrono::high_resolution_clock::now();
  auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
  cout << "Time: " << endl;
  cout << duration.count() << endl;

  cout << "knns" << endl;
  vector<coord_t>::iterator kit = knns.begin();
  for (; kit != knns.end(); kit++)
    cout << *kit << endl;
  
  return 0;
}
