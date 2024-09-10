/*
#
# Hard-sphere collision simulation
# = collision rate Experiment
#
# version 1.0.0
#
# Copyright (c) 2023
# Somrath Kanoksirirath <somrathk@gmail.com>
*/

#include "setup.h"

#define KB_TIMES_AV 8.31446262

Setup::Setup()
{

}

Setup::~Setup()
{
  delete[] param.particle ;
}

void Setup::parse_string(auto *ptr, std::string val, auto func)
{
  try{
    *ptr = func(val);
  }catch (const std::invalid_argument& ia) {
    std::cerr << "Error:: Invalid argument: " << ia.what() << "\n";
  }
}


void Setup::match_string(int *ptr, std::string val, std::string opt, std::vector<std::string> list)
{
  for(size_t i=0 ; i<list.size() ; ++i)
  {
    if( val.compare(list[i])==0 ){
      *ptr = static_cast<int>(i) ;
      return;
    }
  }
  std::cerr << "Error:: " << val << " is NOT a valid option for " << opt << "\n";
}

std::vector<std::string> Setup::split_string(const std::string line, const char delimiter)
{
  std::string temp = line ;
  std::vector<std::string> words ;
  size_t mid = temp.find(delimiter) ;
  while( 0 < mid && mid < temp.length() )
  {
    words.push_back( temp.substr(0, mid) );
    temp = temp.substr(mid+1);
    mid  = temp.find(delimiter);
  }
  if( temp.length() != 0 ){
    words.push_back( temp );
  }

return words ; }


void Setup::parse_string_vec3(float *var, std::string val)
{
    size_t start = val.find('(');
    size_t sep1  = val.find(',', start+1);
    size_t sep2  = val.find(',', sep1+1);
    size_t end   = val.find(')', sep2+1);
    parse_string(var,   val.substr(start+1,sep1), [](std::string x){ return std::stof(x); });
    parse_string(var+1, val.substr( sep1+1,sep2), [](std::string x){ return std::stof(x); });
    parse_string(var+2, val.substr( sep2+1, end), [](std::string x){ return std::stof(x); });
}


void Setup::parse_string_ivec3(int *var, std::string val)
{
    size_t start = val.find('(');
    size_t sep1  = val.find(',', start+1);
    size_t sep2  = val.find(',', sep1+1);
    size_t end   = val.find(')', sep2+1);
    parse_string(var,   val.substr(start+1,sep1), [](std::string x){ return std::stoi(x); });
    parse_string(var+1, val.substr( sep1+1,sep2), [](std::string x){ return std::stoi(x); });
    parse_string(var+2, val.substr( sep2+1, end), [](std::string x){ return std::stoi(x); });
}


void Setup::parse_line(std::string opt, std::string val)
{
  if( opt.compare("TimeStepSize")==0 ){
    float *var = &param.TimeStepSize ;
    parse_string(var, val, [](std::string x){ return std::stof(x); });

  }else if ( opt.compare("NumTimeStep")==0 ){
    unsigned int *var = &param.NumTimeStep ;
    parse_string(var, val, [](std::string x){ return std::stoul(x); });

  }else if ( opt.compare("ShowOutputOnly")==0 ){
    int *var = &param.ShowOutputOnly ;
    std::vector<std::string> list{"TRUE","True","true",".true.","T","t","yes"} ;
    match_string(var, val, opt, list);

  }else if ( opt.compare("NumCell")==0 ){
    parse_string_ivec3(&param.NumCell[0], val);
    param.TotalNumCell = param.NumCell[0]*
                         param.NumCell[1]*
                         param.NumCell[2] ;

  }else if ( opt.compare("CellSize")==0 ){
    parse_string_vec3(&param.CellSize[0], val);

  }else if ( opt.compare("BoxSize")==0 ){
    parse_string_vec3(&param.BoxSize[0], val);

  }else if ( opt.compare("Gravity")==0 ){
    float *var = &param.Gravity ;
    parse_string(var, val, [](std::string x){ return std::stof(x); });

  }else if ( opt.compare("BC_x")==0 ){
    int *var = &param.BC_x ;
    std::vector<std::string> list{"Solid","Periodic"} ;
    match_string(var, val, opt, list);

  }else if ( opt.compare("BC_y")==0 ){
    int *var = &param.BC_y ;
    std::vector<std::string> list{"Solid","Periodic"} ;
    match_string(var, val, opt, list);

  }else if ( opt.compare("BC_z")==0 ){
    int *var = &param.BC_z ;
    std::vector<std::string> list{"Solid","Periodic"} ;
    match_string(var, val, opt, list);

  }else if ( opt.compare("init")==0 ){
    int *var = &param.init ;
    std::vector<std::string> list{"Random","Split_x","Split_y","Split_z"} ;
    match_string(var, val, opt, list);

  }else if ( opt.compare("ntypes")==0 ){
    int *var = &param.num_type ;
    parse_string(var, val, [](std::string x){ return std::stoi(x); });
    param.num_type = *var ;
    param.particle = new ParticleParameters[*var];

  }else if ( opt.compare("num")==0 ){
    if( param.num_type == -1 ){
      std::cerr << "Error:: 'ntypes' is required prior to 'num'.\n" ;
    }
    std::vector<std::string> v = split_string(val, ';');
    param.total_num_particle = 0 ;
    for(int i=0 ; i<param.num_type ; ++i)
    {
      long long int *var = &param.particle[i].num ;
      parse_string(var, v[i], [](std::string x){ return std::stoll(x); });
      param.total_num_particle += param.particle[i].num ;
    }

  }else if ( opt.compare("mass")==0 ){
    if( param.num_type == -1 ){
      std::cerr << "Error:: 'ntypes' is required prior to 'mass'.\n" ;
    }
    std::vector<std::string> v = split_string(val, ';');
    for(int i=0 ; i<param.num_type ; ++i)
    {
      float *var = &param.particle[i].mass ;
      parse_string(var, v[i], [](std::string x){ return std::stof(x); });
    }

  }else if ( opt.compare("velocity")==0 ){
    if( param.num_type == -1 ){
      std::cerr << "Error:: 'ntypes' is required prior to 'velocity'.\n" ;
    }
    std::vector<std::string> v = split_string(val, ';');
    for(int i=0 ; i<param.num_type ; ++i)
    {
      parse_string_vec3(&param.particle[i].velocity[0], v[i]);
    }


  }else if ( opt.compare("temperature")==0 ){
    if( param.num_type == -1 ){
      std::cerr << "Error:: 'ntypes' is required prior to 'temperature'.\n" ;
    }
    std::vector<std::string> v = split_string(val, ';');
    for(int i=0 ; i<param.num_type ; ++i)
    {
      float *var = &param.particle[i].temperature ;
      parse_string(var, v[i], [](std::string x){ return std::stof(x); });
    }

  }else if ( opt.compare("radius")==0 ){
    if( param.num_type == -1 ){
      std::cerr << "Error:: 'ntypes' is required prior to 'radius'.\n" ;
    }
    std::vector<std::string> v = split_string(val, ';');
    for(int i=0 ; i<param.num_type ; ++i)
    {
      float *var = &param.particle[i].radius ;
      parse_string(var, v[i], [](std::string x){ return std::stof(x); });
    }

  }else{
    std::cerr << "ERROR:: Unknown option, " << opt << ", was found in the Setup file.\n" ;
    std::exit(1);
  }

}


bool Setup::parse_cin()
{
  std::ios_base::sync_with_stdio(false); // After this use either std::cin or scanf (MUST NOT use both!!)

  if( !std::cin.rdbuf()->in_avail() ){
    std::cerr << "ERROR:: No Setup file was passed to std::cin." << std::endl;
    return false ;
  }

  // Parse one line
  for(std::string line; std::getline(std::cin, line); )
  {
    // Remove comments (after #)
    size_t end = line.find('#');
    if( end == std::string::npos ){ end = line.length() ; }
    line = line.substr(0, end) ;

    // Remove white spaces
    line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
    if( line.length() == 0 ){ continue; }

    // Split by =
    std::vector<std::string> v = split_string(line, '=');
    if( v.size() == 2 ){
      parse_line(v[0], v[1]);
    }else{
      std::cout << v[0] << std::endl;
      std::cerr << "ERROR:: Incorrect setup line :: " << line << std::endl;
      return false ;
    }
  }

  // Additional checks
  for(int i=0 ; i<3 ; ++i){
    if( param.CellSize[i]*param.NumCell[i] < param.BoxSize[i] )
    {
      std::cout << "WARNING:: NumCell*CellSize < BoxSize in a direction --> Some particles may be lost." << std::endl;
    }
  }
  if( param.total_num_particle % 256 != 0 )
  {
    std::cout << "ERROR:: Total number of particles must be divisible by 256 [Limitation posed by our algorithm]." << std::endl;
    return false ;
  }

  std::ios_base::sync_with_stdio(true);

return true; }


void Setup::printParam()
{
  if( param.ShowOutputOnly > 0 ){ return; }

  std::cout << "\n*** *** Setup *** ***\n"
  << "TimeStepSize = " << param.TimeStepSize << " ns\n"
  << "NumTimeStep  = " << param.NumTimeStep << "\n"
  << "BoxSize   = (" << param.BoxSize[0] << ","
                     << param.BoxSize[1] << ","
                     << param.BoxSize[2] << ") nm\n"
  << "Gravity   = " << param.Gravity << "\n"
  << "BC_x      = " << param.BC_x << " <-- (0) Solid, (1) Periodic \n"
  << "BC_y      = " << param.BC_y << " <-- (0) Solid, (1) Periodic \n"
  << "BC_z      = " << param.BC_z << " <-- (0) Solid, (1) Periodic \n"
  << "NumCell   = (" << param.NumCell[0] << ","
                     << param.NumCell[1] << ","
                     << param.NumCell[2] << ")\n"
  << "CellSize  = (" << param.CellSize[0] << ","
                     << param.CellSize[1] << ","
                     << param.CellSize[2] << ")\n"
  << "init      = " << param.init  << " <-- (0) Random, (1) Split_x, (2) Split_y, (3) Split_z \n"
  << std::endl;

  double coeff = 1.0f/param.BoxSize[0]/param.BoxSize[1]/param.BoxSize[2]/0.60221408 ;

  for(int i=0 ; i<param.num_type ; ++i)
  {
    float V_sigma = sqrt(KB_TIMES_AV
                      *param.particle[i].temperature
                      /param.particle[i].mass) ;
    std::cout << "*** Particle " << i << " *** \n"
    << "num         = " << param.particle[i].num  << "\n"
    << "mass        = " << param.particle[i].mass << " g/mol\n"
    << "density     = " << param.particle[i].num*param.particle[i].mass*coeff << " Kg/m^3\n"
    << "radius      = " << param.particle[i].radius  << " nm\n"
    << "temperature = " << param.particle[i].temperature << " K\n"
    << "V_mean      = (" << param.particle[i].velocity[0] << ","
                         << param.particle[i].velocity[1] << ","
                         << param.particle[i].velocity[2] << ") m/s (nm/ns)\n"
    << "V_sigma     = " << V_sigma << " m/s (nm/ns)\n"
    << std::endl;
  }

  std::cout << "--- --- --- --- ---\n" << std::endl;
}

