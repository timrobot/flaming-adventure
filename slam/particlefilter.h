#ifndef particlefilter_h
#define particlefilter_h

#include <sys/time.h>
#include <vector>
#include <armadillo>
#include "gridmap.h"

typedef struct particle {
  arma::vec pose;
  double life;
} particle_t;

class ParticleFilter {
  private:
    int particle_count;
    particle_t *particles;
    std::vector<arma::vec> latestobs;
    struct timeval obstimestamp;
    
    void weight(particle_t p);
    void resample(void);

  public:
    ParticleFilter(gridmap &map, int nparticles = 1000);
    ~ParticleFilter(void);
    void update(arma::vec &motion, std::vector<arma::vec> &obs);
    arma::vec predict(double *sigma = NULL);
};

#endif
