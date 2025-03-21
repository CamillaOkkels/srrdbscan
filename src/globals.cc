#include <cmath>
#include <cassert>
#include <globals.h>
#include <unistd.h>

double epsilon_original = 0.001;
double epsilon = epsilon_original * epsilon_original;
double approx = 1.0f;
size_t minPts = 500;

std::vector<point*> corePoints;

Metric metric = euclidean;
int (*hashFunc)(const point & a, const BasePoint & b) = &euclideanHash;
double (*distanceFunc)(const point & a, const point & b) = &euclideanDistance;

double angularDistance(const point & a, const point & b)
{
  double result = a.innerProduct(b);

  result = result / (a.norm()*b.norm());

  if(result > 1)
    {
      assert( (result - 1) < 0.00001);
      result = 1;
    }
  
  result = acos(result);

  result = result / 3.14;
  
  return result;
}

double squaredEuclideanDistance(const point & a, const point & b)
{
  return a.squaredEuclideanDistance(b);
}

double euclideanDistance(const point & a, const point & b)
{
  return sqrt(a.squaredEuclideanDistance(b));
}

int euclideanHash(const point & a, const BasePoint & b)
{
  double result = a.innerProduct(b);
  result = result / (4  *  epsilon_original);
  result = std::floor(result);
  return static_cast<int> (result);
}

int angularHash(const point & a, const BasePoint & b)
{
  return (a.innerProduct(b) >= 0 ? 1.0 : -1.0);
}


void parseSRRArguments(int argc, char* argv[], SRRParameters& params)
{
  int opt;

  while ( (opt=getopt(argc, argv, "A:m:d:e:f:M:b:t:K:s:")) != -1  )
    {
      switch(opt)
	{
	case 'm':
	  minPts = std::stoi(optarg);
	  break;
	case 'e':
	  epsilon_original = std::stod(optarg);
	  epsilon = epsilon_original;
	  break;
	case 'A':
	  approx = std::stod(optarg);
	  break;
	case 'd':
		params.delta = std::stod(optarg);
		break;
	case 'f':
	  params.fileName = std::string(optarg);
	  break;
	case 't':
	  params.numberOfThreads = std::stoi(optarg);
	  break;
	case 'M':
	  params.memoConstraint = std::stod(optarg);
	  break;
	case 'b':
	  params.baselineFileName = std::string(optarg);
	  break;
	case 'K':
	  params.level = std::stoi(optarg);
	  break;
	case 's':
	  params.shrinkageFactor = std::stod(optarg);
	  break;
	default:
	  std::cout << "Usage: " << argv[0] << " "
		    << "-f inputFile "
		    << "[-M memoryConstraint ] "
			<< "[-A approximationFactor]"
		    << "[-e eps] " 
		    << "[-m minPts] "
			<< "[-t #threads] "
			<< "[-d delta]"
		    << "[-b baselineClusteringFile]"
			<< "[-K baselevel]"
			<< "[-s shrinkageFactor [0,1]]"
		    << std::endl;
	  exit(EXIT_FAILURE);
	}
    }
  if (params.fileName.empty())
    {
      std::cerr << "-f inputFile is mandatory" << std::endl;
      exit(EXIT_FAILURE);
    }
  if (metric == euclidean)
    {
      epsilon = epsilon_original * epsilon_original;
      distanceFunc = squaredEuclideanDistance;
    }
}
