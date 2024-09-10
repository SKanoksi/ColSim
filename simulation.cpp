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


#include "simulation.h"

#define DATATYPE GLfloat
#define KB_TIMES_AV 8.31446262   // Gas constant = KB*AV

Simulation::Simulation()
{

}


bool Simulation::init()
{
    MySetup = new Setup();
    if( !MySetup->parse_cin() ){ return false; }
    MySetup->printParam();

    MyParticle = new Particle();
    // init(Arrow size)
    if( !MyParticle->init(0.250f) ){ return false; }

    MyBox = new Box();
    // init(Lx, Ly, Lz) --> each = [-L/2, L/2]
    if( !MyBox->init(MySetup->param.BoxSize[0], MySetup->param.BoxSize[1], MySetup->param.BoxSize[2]) ){ return false; }

    this->box_size_x = MySetup->param.BoxSize[0] ;

    // ********************************

    // *** Initialization ***
    this->nt = 0 ;

    GLuint num_hit[7] ;
    for(int i=0 ; i<7 ; ++i){ num_hit[i] = 0 ; }

    this->num_particle = MySetup->param.total_num_particle ;
    auto random_device = std::random_device{};
    auto random_generator = std::default_random_engine{ random_device() };
    const unsigned int nL = (unsigned int)std::ceil(std::pow(num_particle, (double)1/3));

    // Create particle tag
    std::vector<short> tag(num_particle, 0);
    long long int num = 0 ;
    for(int n=0 ; n<MySetup->param.num_type ; ++n){
      for(int i=0 ; i<MySetup->param.particle[n].num ; ++i){
        tag[num] = static_cast<short>(n) ;
        ++num ;
      }
    }
    if( MySetup->param.init == 0 ){
      std::shuffle(tag.begin(), tag.end(), random_generator);
    }

    // Memory allocation
    // vec4(pos, radius),  vec4(velocity, mass)
    std::vector<DATATYPE> particles(8*num_particle, static_cast<DATATYPE>(0)) ;
    std::vector<GLuint> count(MySetup->param.TotalNumCell, (GLuint)0);

    unsigned int *ix, *iy, *iz ;
    unsigned int ipos[3] ;
    if( MySetup->param.init == 1 ){
      ix = &ipos[2] ;
      iy = &ipos[1] ;
      iz = &ipos[0] ;
    }else if( MySetup->param.init == 2 ){
      ix = &ipos[1] ;
      iy = &ipos[2] ;
      iz = &ipos[0] ;
    }else{
      ix = &ipos[0] ;
      iy = &ipos[1] ;
      iz = &ipos[2] ;
    }

    for(long long int i=0 ; i<num_particle ; ++i)
    {
      long long int j = 8*i ;
      // vec4(pos, radius)
      ipos[0] = i%nL ;
      unsigned int iremain = (i-ipos[0])/nL ;
      ipos[1] = iremain%nL ;
      ipos[2] = (iremain - ipos[1])/nL ;

      particles[j]   = static_cast<DATATYPE>(( (DATATYPE)(*ix)/(nL-1) - 0.5f )*( MySetup->param.BoxSize[0]*(nL-1)/nL )) ;
      particles[j+1] = static_cast<DATATYPE>(( (DATATYPE)(*iy)/(nL-1) - 0.5f )*( MySetup->param.BoxSize[1]*(nL-1)/nL )) ;
      particles[j+2] = static_cast<DATATYPE>(( (DATATYPE)(*iz)/(nL-1) - 0.5f )*( MySetup->param.BoxSize[2]*(nL-1)/nL )) ;
      particles[j+3] = MySetup->param.particle[tag[i]].radius ;

      // vec4(velocity, mass)
      j += 4 ;
      float sigma = sqrt(KB_TIMES_AV
                         *MySetup->param.particle[tag[i]].temperature
                         /MySetup->param.particle[tag[i]].mass) ; // g/mol to kg/mol
      for(int k=0 ; k<3 ; ++k){
        std::normal_distribution<GLfloat> vel_dist(MySetup->param.particle[tag[i]].velocity[k], sigma);
        particles[j+k] = vel_dist(random_generator) ;
      }
      particles[j+3] = MySetup->param.particle[tag[i]].mass ;
    }

    // ********************************

    this->buildComShader();

    // Input Buffer
    glGenBuffers(2, &VBO[0]);
    glGenBuffers(1, &countVBO);
    glGenBuffers(1, &ptrVBO);
    glGenBuffers(1, &hitVBO);
    glGenBuffers(1, &meaVBO);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, VBO[0]);
      glBufferData(GL_SHADER_STORAGE_BUFFER, particles.size()*sizeof(DATATYPE), &particles[0], GL_STREAM_DRAW);
      glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, VBO[0]);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, VBO[1]);
      glBufferData(GL_SHADER_STORAGE_BUFFER, particles.size()*sizeof(DATATYPE), nullptr, GL_STREAM_DRAW);
      glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, VBO[1]);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, countVBO);
      glBufferData(GL_SHADER_STORAGE_BUFFER, count.size()*sizeof(GLuint), &count[0], GL_STREAM_DRAW);
      glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, countVBO);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ptrVBO);
      glBufferData(GL_SHADER_STORAGE_BUFFER, count.size()*sizeof(GLuint), nullptr, GL_STREAM_DRAW);
      glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, ptrVBO);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, hitVBO);
      glBufferData(GL_SHADER_STORAGE_BUFFER, 7*sizeof(GLuint), &num_hit[0], GL_STREAM_DRAW);
      glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, hitVBO);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, meaVBO);
      glBufferData(GL_SHADER_STORAGE_BUFFER, 8*sizeof(GLfloat), nullptr, GL_STREAM_DRAW);
      glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, meaVBO);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);


    MyParticle->set(VBO[0]);

    // Output format
    std::cout << "nt,num_collision,num_hit_lx,num_hit_rx,num_hit_ly,num_hit_ry,num_hit_lz,num_hit_rz," ;
    for(int i=0 ; i<MySetup->param.num_type ; ++i){
        std::cout
        << "net_mass_" << i << ","
        << "net_vx_" << i << ","
        << "net_vy_" << i << ","
        << "net_vz_" << i << ","
        << "net_px_" << i << ","
        << "net_py_" << i << ","
        << "net_pz_" << i << ","
        << "net_ke_x_" << i << ","
        << "net_ke_y_" << i << ","
        << "net_ke_z_" << i << ","
        << "net_ke_"  << i << ","
        << "net_num_" << i << "," ;
    }
    std::cout << "total_ke," << std::endl;


return true;}


void Simulation::run()
{
    if( this->nt < MySetup->param.NumTimeStep )
    {

    // 1. Forward
    glUseProgram(Forward_ComSh);
      glDispatchCompute( (GLuint)ceil(num_particle/256), 256, 1);
      glMemoryBarrier(GL_ALL_BARRIER_BITS);

    for(unsigned int i=0 ; i<8 ; ++i)
    {
      DATATYPE X_OFFSET = (i%2==0) ? -(MySetup->param.CellSize[0])/2 : 0.f ;
      DATATYPE Y_OFFSET = (i%4<2)  ? -(MySetup->param.CellSize[1])/2 : 0.f ;
      DATATYPE Z_OFFSET = (i<4)    ? -(MySetup->param.CellSize[2])/2 : 0.f ;

      // 2.1 Histogram
      glUseProgram(Histogram_ComSh);
        glUniform3f(glGetUniformLocation(Histogram_ComSh,"Shift"), X_OFFSET, Y_OFFSET, Z_OFFSET);
        glDispatchCompute( (GLuint)ceil(num_particle/256), 32, 1);
        glMemoryBarrier(GL_ALL_BARRIER_BITS);
      // 2.2 Rolling sum
      glUseProgram(RollingSum_Wrap_ComSh);
        glDispatchCompute( (GLuint)ceil(MySetup->param.TotalNumCell/256), 1, 1);
        glMemoryBarrier(GL_ALL_BARRIER_BITS);
      glUseProgram(RollingSum_Global_ComSh);
        glDispatchCompute(1, 1, 1);
        glMemoryBarrier(GL_ALL_BARRIER_BITS);
      // 2.3 Rearrange
      glUseProgram(Rearrange_ComSh);
        glUniform3f(glGetUniformLocation(Rearrange_ComSh,"Shift"), X_OFFSET, Y_OFFSET, Z_OFFSET);
        glDispatchCompute( (GLuint)ceil(num_particle/256), 256, 1);
        glMemoryBarrier(GL_ALL_BARRIER_BITS);

      // 3. Collision
      glUseProgram(Collision_ComSh);
        glDispatchCompute( (GLuint)MySetup->param.TotalNumCell, 1, 1);
        glMemoryBarrier(GL_ALL_BARRIER_BITS);

      // Swap buffers
      if( i%2 == 0 ){
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, VBO[1]);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, VBO[0]);
      }else{
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, VBO[0]);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, VBO[1]);
      }
    }


    // Get hits count
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, hitVBO);
    {
      GLuint *ptr = (GLuint*)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_WRITE);
      // Print
      std::cout << this->nt << "," ;
      for(int i=0 ; i<7 ; ++i){
        std::cout << ptr[i] << "," ;
        ptr[i] = 0 ;
      }
      glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
    }


    // 4. Measurements
    GLfloat total_KE = 0. ; // of system

    GLfloat min_mass = 0. ;
    GLfloat max_mass ;
    for(int i=0 ; i<MySetup->param.num_type ; ++i)
    {
      // 4.1 Calculate parameters/measurements
      glUseProgram(CalParam_ComSh);
        if( i < MySetup->param.num_type-1 ){
          max_mass = 0.5*(MySetup->param.particle[i].mass + MySetup->param.particle[i+1].mass) ;
        }else{
          max_mass = 1.1*MySetup->param.particle[i].mass ;
        }
        glUniform2f(glGetUniformLocation(CalParam_ComSh,"mass_range"), min_mass, max_mass);
        glDispatchCompute( (GLuint)ceil(num_particle/256), 256, 1);
        min_mass = max_mass ;
        glMemoryBarrier(GL_ALL_BARRIER_BITS);

      // 4.2 Reduction
      glUseProgram(Reduction_ComSh);
        glDispatchCompute( 1, 1, 1);
        glMemoryBarrier(GL_ALL_BARRIER_BITS);

      // Get measurements
      glBindBuffer(GL_SHADER_STORAGE_BUFFER, meaVBO);
      {
        GLfloat *ptr = (GLfloat*)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_WRITE);
        GLfloat m = MySetup->param.particle[i].mass ;
        GLfloat net_KE = 0.5*m*(ptr[4]+ptr[5]+ptr[6]) ;
        std::cout
        <<       ptr[0] << ","
        <<       ptr[1] << ","
        <<       ptr[2] << ","
        <<       ptr[3] << ","
        <<     m*ptr[1] << ","
        <<     m*ptr[2] << ","
        <<     m*ptr[3] << ","
        << 0.5*m*ptr[4] << ","
        << 0.5*m*ptr[5] << ","
        << 0.5*m*ptr[6] << ","
        <<       net_KE << ","
        <<       ptr[7] << "," ;
        total_KE += net_KE ;
        glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
      }

    }
    std::cout << total_KE << "," << std::endl;

    // Finish 1 timestep
    this->nt += 1 ;

    }else{
      glUseProgram(0);
      glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    }
}


void Simulation::draw()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    MyParticle->draw(num_particle);
    MyBox->draw();
}


Simulation::~Simulation()
{
    glDeleteProgram(Forward_ComSh);
    glDeleteProgram(Histogram_ComSh);
    glDeleteProgram(RollingSum_Wrap_ComSh);
    glDeleteProgram(RollingSum_Global_ComSh);
    glDeleteProgram(Rearrange_ComSh);
    glDeleteProgram(Collision_ComSh);
    glDeleteProgram(CalParam_ComSh);
    glDeleteProgram(Reduction_ComSh);

    glDeleteBuffers(2, &VBO[0]);
    glDeleteBuffers(1, &countVBO);
    glDeleteBuffers(1, &ptrVBO);
    glDeleteBuffers(1, &hitVBO);
    glDeleteBuffers(1, &meaVBO);

    delete MySetup ;
    delete MyParticle ;
    delete MyBox ;
}


// ********************************

void Simulation::buildComShader()
{
    std::string comCode, BC_code, temp ;

    // 1. Forward
    if( !readSource("./ComSh/Forward.comSh", comCode) ){ // The code must be found ###
      std::cout << "ERROR:: ./ComSh/Forward.comSh was not found.\n" ;
    }

    float gdt = MySetup->param.TimeStepSize*MySetup->param.Gravity*1E-9 ; // m/s^2 to nm/ns^2
    ToString(gdt, temp);
    comCode.replace( comCode.find("GDT"), 3,  temp.c_str() );
    ToString(gdt/2, temp);
    comCode.replace( comCode.find("HALF_GDT"), 8,  temp.c_str() );

    ToString( MySetup->param.TimeStepSize, temp);
    comCode.replace( comCode.find("TIME_STEP_SIZE"), 14,  temp.c_str() );

    // Boundary conditions
    const std::string periodic_template =
    "while( new_pos.XXX < -LENGTH ){ new_pos.XXX += BOX_SIZE ; atomicAdd(num_hit_XXX[0], 1); }\n"
    "while( new_pos.XXX >  LENGTH ){ new_pos.XXX -= BOX_SIZE ; atomicAdd(num_hit_XXX[1], 1); }\n" ;
    const std::string solidwall_template =
    "if( new_pos.XXX - pos_R.w < -LENGTH ){ new_vel.XXX *= -1 ; atomicAdd(num_hit_XXX[0], 1); }\n"
    "if( new_pos.XXX + pos_R.w >  LENGTH ){ new_vel.XXX *= -1 ; atomicAdd(num_hit_XXX[1], 1); }\n" ;

    // BC_X
    if( MySetup->param.BC_x == 0 ){
      BC_code = solidwall_template ;
    }else{
      BC_code = periodic_template ;
      ToString( MySetup->param.BoxSize[0], temp);
      for(int i=0 ; i<2 ; ++i){
        BC_code.replace( BC_code.find("BOX_SIZE"), 8, temp.c_str() );
      }
    }
    for(int i=0 ; i<6 ; ++i){
      BC_code.replace( BC_code.find("XXX"), 3, "x");
    }
    ToString( MySetup->param.BoxSize[0]/2, temp);
    for(int i=0 ; i<2 ; ++i){
      BC_code.replace( BC_code.find("LENGTH"), 6,  temp.c_str() );
    }
    comCode.replace( comCode.find("BC_X"), 4,  BC_code.c_str() );

    // BC_Y
    if( MySetup->param.BC_y == 0 ){
      BC_code = solidwall_template ;
    }else{
      BC_code = periodic_template ;
      ToString( MySetup->param.BoxSize[1], temp);
      for(int i=0 ; i<2 ; ++i){
        BC_code.replace( BC_code.find("BOX_SIZE"), 8, temp.c_str() );
      }
    }
    for(int i=0 ; i<6 ; ++i){
      BC_code.replace( BC_code.find("XXX"), 3, "y");
    }
    ToString( MySetup->param.BoxSize[1]/2, temp);
    for(int i=0 ; i<2 ; ++i){
      BC_code.replace( BC_code.find("LENGTH"), 6,  temp.c_str() );
    }
    comCode.replace( comCode.find("BC_Y"), 4,  BC_code.c_str() );

    // BC_Z
    if( MySetup->param.BC_z == 0 ){
      BC_code = solidwall_template ;
    }else{
      BC_code = periodic_template ;
      ToString( MySetup->param.BoxSize[2], temp);
      for(int i=0 ; i<2 ; ++i){
        BC_code.replace( BC_code.find("BOX_SIZE"), 8, temp.c_str() );
      }
    }
    for(int i=0 ; i<6 ; ++i){
      BC_code.replace( BC_code.find("XXX"), 3, "z");
    }
    ToString( MySetup->param.BoxSize[2]/2, temp);
    for(int i=0 ; i<2 ; ++i){
      BC_code.replace( BC_code.find("LENGTH"), 6,  temp.c_str() );
    }
    comCode.replace( comCode.find("BC_Z"), 4,  BC_code.c_str() );

    Forward_ComSh = createComSh(comCode.c_str(), comCode.length());


    // 2.1 Histogram
    if( !readSource("./ComSh/Histogram.comSh", comCode) ){ // The code must be found ###
      std::cout << "ERROR:: ./ComSh/Histogram.comSh was not found.\n" ;
    }

    ToString( 8, temp);
    comCode.replace( comCode.find("NUM_REPEAT"), 10,  temp.c_str() );

    ToString( MySetup->param.BoxSize[0]/2, temp);
    comCode.replace( comCode.find("OFFSET_X"), 8,  temp.c_str() );
    ToString( MySetup->param.BoxSize[1]/2, temp);
    comCode.replace( comCode.find("OFFSET_Y"), 8,  temp.c_str() );
    ToString( MySetup->param.BoxSize[2]/2, temp);
    comCode.replace( comCode.find("OFFSET_Z"), 8,  temp.c_str() );

    ToString( MySetup->param.CellSize[0], temp);
    comCode.replace( comCode.find("STRIDE_X"), 8,  temp.c_str() );
    ToString( MySetup->param.CellSize[1], temp);
    comCode.replace( comCode.find("STRIDE_Y"), 8,  temp.c_str() );
    ToString( MySetup->param.CellSize[2], temp);
    comCode.replace( comCode.find("STRIDE_Z"), 8,  temp.c_str() );

    ToString( MySetup->param.NumCell[0], temp);
    comCode.replace( comCode.find("NL_BOX_X"), 8,  temp.c_str() );
    ToString( MySetup->param.NumCell[1], temp);
    comCode.replace( comCode.find("NL_BOX_Y"), 8,  temp.c_str() );

    Histogram_ComSh = createComSh(comCode.c_str(), comCode.length());


    // 2.2 Rolling sum
    // RollingSum_Wrap
    if( !readSource("./ComSh/RollingSum_Wrap.comSh", comCode) ){ // The code must be found ###
      std::cout << "ERROR:: ./ComSh/RollingSum_Wrap.comSh was not found.\n" ;
    }

    RollingSum_Wrap_ComSh = createComSh(comCode.c_str(), comCode.length());

    // RollingSum_Global
    if( !readSource("./ComSh/RollingSum_Global.comSh", comCode) ){ // The code must be found ###
      std::cout << "ERROR:: ./ComSh/RollingSum_Global.comSh was not found.\n" ;
    }

    ToString( (GLuint)ceil(MySetup->param.TotalNumCell/256), temp);
    comCode.replace( comCode.find("NUM_REPEAT"), 10,  temp.c_str() );

    RollingSum_Global_ComSh = createComSh(comCode.c_str(), comCode.length());


    // 2.3 Rearrange
    if( !readSource("./ComSh/Rearrange.comSh", comCode) ){ // The code must be found ###
      std::cout << "ERROR:: ./ComSh/Rearrange.comSh was not found.\n" ;
    }

    ToString( MySetup->param.BoxSize[0]/2, temp);
    comCode.replace( comCode.find("OFFSET_X"), 8,  temp.c_str() );
    ToString( MySetup->param.BoxSize[1]/2, temp);
    comCode.replace( comCode.find("OFFSET_Y"), 8,  temp.c_str() );
    ToString( MySetup->param.BoxSize[2]/2, temp);
    comCode.replace( comCode.find("OFFSET_Z"), 8,  temp.c_str() );

    ToString( MySetup->param.CellSize[0], temp);
    comCode.replace( comCode.find("STRIDE_X"), 8,  temp.c_str() );
    ToString( MySetup->param.CellSize[1], temp);
    comCode.replace( comCode.find("STRIDE_Y"), 8,  temp.c_str() );
    ToString( MySetup->param.CellSize[2], temp);
    comCode.replace( comCode.find("STRIDE_Z"), 8,  temp.c_str() );

    ToString( MySetup->param.NumCell[0], temp);
    comCode.replace( comCode.find("NL_BOX_X"), 8,  temp.c_str() );
    ToString( MySetup->param.NumCell[1], temp);
    comCode.replace( comCode.find("NL_BOX_Y"), 8,  temp.c_str() );

    Rearrange_ComSh = createComSh(comCode.c_str(), comCode.length());

    // 3. Collision
    if( !readSource("./ComSh/Collision.comSh", comCode) ){ // The code must be found ###
      std::cout << "ERROR:: ./ComSh/Collision.comSh was not found.\n" ;
    }

    ToString( MySetup->param.TimeStepSize, temp);
    comCode.replace( comCode.find("TIME_STEP_SIZE"), 14,  temp.c_str() );

    Collision_ComSh = createComSh(comCode.c_str(), comCode.length());

    // 4.1 Calculate parameters/measurements
    if( !readSource("./ComSh/CalParam.comSh", comCode) ){ // The code must be found ###
      std::cout << "ERROR:: ./ComSh/CalParam.comSh was not found.\n" ;
    }
    CalParam_ComSh = createComSh(comCode.c_str(), comCode.length());

    // 4.2 Reduction
    if( !readSource("./ComSh/Lazy_Reduction.comSh", comCode) ){ // The code must be found ###
      std::cout << "ERROR:: ./ComSh/Lazy_Reduction.comSh was not found.\n" ;
    }

    ToString( ceil(num_particle/256), temp);
    comCode.replace( comCode.find("NUM_REPEAT"), 10,  temp.c_str() );

    Reduction_ComSh = createComSh(comCode.c_str(), comCode.length());

}



