#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>

struct data_type_particle
	{
		int x,y,spin;
	};

struct data_type_grid
	{
		int density,magnetization; 
	};

void initialize(struct data_type_particle*, struct data_type_grid**, int, int, int);
int generate_random_int(int);
float generate_random_float(void);
void time_evolve(struct data_type_particle*, struct data_type_grid**, int, int, int, float, float, float, float);

void main(void)
	{
		srand(time(0));
		FILE *output_density, *output_magnetization, *output_den_profile, *output_mag_profile;
		char file_name_density[100], file_name_mag[100], file_name_den_profile[100], file_name_mag_profile[100];
		int count = 0, file_name_return;

		//Definitions of initial conditions.
		int Lx,Ly,n,den_profile, mag_profile;
		float beta,avg_density,D,epsilon;
		double total_time,time_step,current_time,prev_record,update_time;

		Lx = 100;
		Ly = 100;
		avg_density = 3.0;

		n = avg_density*Lx*Ly;

		beta = 1.8;
		D = 1.0;
		epsilon = 0.9;

		total_time = 1e8;
		current_time = 0;
		update_time = 2.0;
		time_step = 1.0/(4*D+exp(beta));

		//struct data_type_particle particles[n];
		//struct data_type_grid grid[Lx][Ly];

		struct data_type_particle *particles;
		particles = (struct data_type_particle*) calloc(n,sizeof(struct data_type_particle));

		struct data_type_grid **grid;
		grid = (struct data_type_grid**) calloc(Lx,sizeof(struct data_type_grid*));
		for(int i=0;i<Lx;i++)
			{
				grid[i] = (struct data_type_grid*) calloc(Ly,sizeof(struct data_type_grid));
				for(int j=0;j<Ly;j++)
					{
						grid[i][j].density=0;
						grid[i][j].magnetization=0;
					}
			}

		initialize(particles, grid, n, Lx, Ly);
		printf("Initialization Complete!\n");

		sprintf(file_name_density, "%i_density.txt", count);
		sprintf(file_name_mag, "%i_mag.txt", count);
		sprintf(file_name_den_profile, "%i_den_profile.txt", count);
		sprintf(file_name_mag_profile, "%i_mag_profile.txt", count);

		output_density = fopen(file_name_density,"w");
		output_magnetization = fopen(file_name_mag,"w");
		output_den_profile = fopen(file_name_den_profile,"w");
		output_mag_profile = fopen(file_name_mag_profile,"w");

		for(int i=0;i<Lx;i++)
			{
				den_profile = 0;
				mag_profile = 0;
				for(int j=0;j<Ly;j++)
					{
						den_profile += grid[i][j].density;
						mag_profile += grid[i][j].magnetization;
						fprintf(output_density,"%i\t",grid[j][i].density);
						fprintf(output_magnetization,"%i\t",grid[j][i].magnetization);
					}
				fprintf(output_density,"\n");
				fprintf(output_magnetization,"\n");
				fprintf(output_den_profile,"%i\t%lf\n",i,(double)den_profile/Ly);
				fprintf(output_mag_profile,"%i\t%lf\n",i,(double)mag_profile/Ly);
				prev_record = 0.0;
			}

		fclose(output_density);
		fclose(output_magnetization);
		fclose(output_den_profile);
		fclose(output_mag_profile);

		count++;
		
		while(current_time<total_time)
			{
				
				//printf("Current time:%lf\n",current_time);
				time_evolve(particles,grid,n,Lx,Ly,time_step,beta,D,epsilon);
				
				if(current_time>prev_record+update_time)
					{
						sprintf(file_name_density, "%i_density.txt", count);
						sprintf(file_name_mag, "%i_mag.txt", count);
						sprintf(file_name_den_profile, "%i_den_profile.txt", count);
						sprintf(file_name_mag_profile, "%i_mag_profile.txt", count);
						
						output_density = fopen(file_name_density,"w");
						output_magnetization = fopen(file_name_mag,"w");
						output_den_profile = fopen(file_name_den_profile,"w");
						output_mag_profile = fopen(file_name_mag_profile,"w");

						printf("Current Time:%lf\n",current_time);
						for(int i=0;i<Lx;i++)
							{
								den_profile = 0;
								mag_profile = 0;
								
								for(int j=0;j<Ly;j++)
									{
										den_profile += grid[i][j].density;
										mag_profile += grid[i][j].magnetization;
										fprintf(output_density,"%i\t",grid[j][i].density);
										fprintf(output_magnetization,"%i\t",grid[j][i].magnetization);
									}
								fprintf(output_density,"\n");
								fprintf(output_magnetization,"\n");
								fprintf(output_den_profile,"%i\t%lf\n",i,(double)den_profile/Ly);
								fprintf(output_mag_profile,"%i\t%lf\n",i,(double)mag_profile/Ly);
								
							}
						prev_record = current_time;
						fprintf(output_density,"\n");
						fprintf(output_magnetization,"\n");
						
						fclose(output_density);
						fclose(output_magnetization);
						fclose(output_den_profile);
						fclose(output_mag_profile);

						count++;
					}

				current_time += time_step/n;
			}


		printf("Done!\n");
		printf("Lx:%i\tLy:%i\tn:%i\tAverage Density:%f\nBeta:%f\tD: %f\tEpsilon:%f\n",Lx,Ly,n,avg_density,beta,D,epsilon);

	}

int generate_random_int(int max)
	{
		return rand()%max;
	}

void initialize(struct data_type_particle* particles, struct data_type_grid** grid, int n, int Lx, int Ly)
	{
		int temp_x,temp_y,temp_spin;
		printf("Begin initialize!\n");

		for(int i=0;i<n;i++)
			{
				temp_x = generate_random_int(Lx);
				temp_y = generate_random_int(Ly);
				if(generate_random_int(2) == 0)
					temp_spin = -1;
				else
					temp_spin = 1;
				
				particles[i].x = temp_x;
				particles[i].y = temp_y;
				particles[i].spin = temp_spin;

				grid[temp_x][temp_y].density++;
				grid[temp_x][temp_y].magnetization += temp_spin;
			}

	}

float generate_random_float(void)
	{
		return 1.0*rand()/RAND_MAX;
	}	

void time_evolve(struct data_type_particle* particles, struct data_type_grid** grid, int n, int Lx, int Ly, float time_step, float beta, float D, float epsilon)
	{
		int temp_x,temp_y,temp_spin,random_particle;
		float W,temp_magnetization,temp_density,random_float;
		
		random_particle = generate_random_int(n);
		temp_x = particles[random_particle].x;
		temp_y = particles[random_particle].y;
		temp_spin = particles[random_particle].spin;
		temp_density = grid[temp_x][temp_y].density;
		temp_magnetization = grid[temp_x][temp_y].magnetization;

		W = exp(-1.0*temp_spin*beta*temp_magnetization/temp_density); //Spin flip probability

		random_float = generate_random_float();

		if(random_float>(W+4*D)*time_step) //Condition to do nothing.
			{
				//Do nothing. This might speed up the simulation
			}

		else if(random_float<=W*time_step)		//Spin flip
			{
				particles[random_particle].spin *= -1;
				grid[temp_x][temp_y].magnetization += -2*temp_spin;
			}	

		else if(random_float>W*time_step && random_float<=(W+D)*time_step) //Upward hop
			{
				grid[temp_x][temp_y].density--;
				grid[temp_x][temp_y].magnetization -= temp_spin;
				
				if(temp_y == Ly-1) //Checking for end of boundary to implement periodic boundary conditions
					{
						particles[random_particle].y = 0;
						grid[temp_x][0].density++;
						grid[temp_x][0].magnetization += temp_spin;
					}
				else
					{
						particles[random_particle].y++;
						grid[temp_x][temp_y+1].density++;
						grid[temp_x][temp_y+1].magnetization += temp_spin;
					}
			}

		else if(random_float>(W+D)*time_step && random_float<=(W+2*D)*time_step) //Downward hop
			{
				grid[temp_x][temp_y].density--;
				grid[temp_x][temp_y].magnetization -= temp_spin;
				
				if(temp_y == 0) //Checking for end of boundary to implement periodic boundary conditions
					{
						particles[random_particle].y = Ly-1;
						grid[temp_x][Ly-1].density++;
						grid[temp_x][Ly-1].magnetization += temp_spin;
					}
				else
					{
						particles[random_particle].y--;
						grid[temp_x][temp_y-1].density++;
						grid[temp_x][temp_y-1].magnetization += temp_spin;
					}
			}

		else if(random_float>(W+2*D)*time_step && random_float<=(W+3*D+D*temp_spin*epsilon)*time_step) //Rightward hop
			{
				grid[temp_x][temp_y].density--;
				grid[temp_x][temp_y].magnetization -= temp_spin;
				
				if(temp_x == Lx-1) //Checking for end of boundary to implement periodic boundary conditions
					{
						particles[random_particle].x = 0;
						grid[0][temp_y].density++;
						grid[0][temp_y].magnetization += temp_spin;
					}
				else
					{
						particles[random_particle].x++;
						grid[temp_x+1][temp_y].density++;
						grid[temp_x+1][temp_y].magnetization += temp_spin;
					}
			}

		else if(random_float>(W+3*D+D*temp_spin*epsilon)*time_step && random_float<=(W+4*D)*time_step) //Leftward hop
			{
				grid[temp_x][temp_y].density--;
				grid[temp_x][temp_y].magnetization -= temp_spin;

				if(temp_x == 0) //Checking for end of boundary to implement periodic boundary conditions
					{
						particles[random_particle].x = Lx-1;
						grid[Lx-1][temp_y].density++;
						grid[Lx-1][temp_y].magnetization += temp_spin;
					}
				else
					{
						particles[random_particle].x--;
						grid[temp_x-1][temp_y].density++;
						grid[temp_x-1][temp_y].magnetization += temp_spin;
					}
			}

	}