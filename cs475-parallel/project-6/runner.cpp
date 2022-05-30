// 1. Program header

// set desired opencl version before including headers
#define CL_TARGET_OPENCL_VERSION 110

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <omp.h>
#include <unistd.h>

#include "CL/cl.h"
#include "CL/cl_platform.h"

#ifndef NUM_ELEMENTS
#define NUM_ELEMENTS (64 * 1024 * 1024)
#endif

#ifndef LOCAL_SIZE
#define LOCAL_SIZE 64
#endif

#define NUM_WORK_GROUPS NUM_ELEMENTS / LOCAL_SIZE

#ifndef CL_FILE_NAME
#define CL_FILE_NAME "array-mult.cl"
#endif

void Wait(cl_command_queue);
int LookAtTheBits(float);

int main(int argc, char *argv[]) {
  // see if we can even open the opencl kernel program
  // (no point going on if we can't):

  FILE *fp = fopen(CL_FILE_NAME, "r");
  if (fp == NULL) {
    fprintf(stderr, "Cannot open OpenCL source file '%s'\n", CL_FILE_NAME);
    return 1;
  }

  cl_int status; // returned status from opencl calls
                 // test against CL_SUCCESS

  // get the platform id:

  cl_platform_id platform;
  status = clGetPlatformIDs(1, &platform, NULL);
  if (status != CL_SUCCESS)
    fprintf(stderr, "clGetPlatformIDs failed (2)\n");

  // get the device id:

  cl_device_id device;
  status = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);
  if (status != CL_SUCCESS)
    fprintf(stderr, "clGetDeviceIDs failed (2)\n");

  // 2. allocate the host memory buffers:

  float *host_A = new float[NUM_ELEMENTS];
  float *host_B = new float[NUM_ELEMENTS];
  float *host_C = new float[NUM_ELEMENTS];
  float *host_D = new float[NUM_ELEMENTS];

  // fill the host memory buffers:

  for (int i = 0; i < NUM_ELEMENTS; i++) {
    host_A[i] = host_B[i] = host_C[i] = (float)sqrt((double)i);
  }

  size_t dataSize = NUM_ELEMENTS * sizeof(float);

  // 3. create an opencl context:

  cl_context context = clCreateContext(NULL, 1, &device, NULL, NULL, &status);
  if (status != CL_SUCCESS)
    fprintf(stderr, "clCreateContext failed\n");

  // 4. create an opencl command queue:

  cl_command_queue cmdQueue = clCreateCommandQueue(context, device, 0, &status);
  if (status != CL_SUCCESS)
    fprintf(stderr, "clCreateCommandQueue failed\n");

  // 5. allocate the device memory buffers:

  cl_mem device_A =
      clCreateBuffer(context, CL_MEM_READ_ONLY, dataSize, NULL, &status);
  if (status != CL_SUCCESS)
    fprintf(stderr, "clCreateBuffer failed (1)\n");

  cl_mem device_B =
      clCreateBuffer(context, CL_MEM_READ_ONLY, dataSize, NULL, &status);
  if (status != CL_SUCCESS)
    fprintf(stderr, "clCreateBuffer failed (2)\n");

  cl_mem device_C =
      clCreateBuffer(context, CL_MEM_WRITE_ONLY, dataSize, NULL, &status);
  if (status != CL_SUCCESS)
    fprintf(stderr, "clCreateBuffer failed (3)\n");

  cl_mem device_D =
      clCreateBuffer(context, CL_MEM_WRITE_ONLY, dataSize, NULL, &status);
  if (status != CL_SUCCESS)
    fprintf(stderr, "clCreateBuffer failed (4)\n");

  // 6. enqueue the 2 commands to write the data from the host buffers to the
  // device buffers:

  status = clEnqueueWriteBuffer(cmdQueue, device_A, CL_FALSE, 0, dataSize,
                                host_A, 0, NULL, NULL);
  if (status != CL_SUCCESS)
    fprintf(stderr, "clEnqueueWriteBuffer failed (1)\n");

  status = clEnqueueWriteBuffer(cmdQueue, device_B, CL_FALSE, 0, dataSize,
                                host_B, 0, NULL, NULL);
  if (status != CL_SUCCESS)
    fprintf(stderr, "clEnqueueWriteBuffer failed (2)\n");

  status = clEnqueueWriteBuffer(cmdQueue, device_C, CL_FALSE, 0, dataSize,
                                host_C, 0, NULL, NULL);
  if (status != CL_SUCCESS)
    fprintf(stderr, "clEnqueueWriteBuffer failed (3)\n");

  Wait(cmdQueue);

  // 7. read the kernel code from a file:

  fseek(fp, 0, SEEK_END);
  size_t fileSize = ftell(fp);
  fseek(fp, 0, SEEK_SET);
  char *clProgramText = new char[fileSize + 1]; // leave room for '\0'
  size_t n = fread(clProgramText, 1, fileSize, fp);
  clProgramText[fileSize] = '\0';
  fclose(fp);
  if (n != fileSize)
    fprintf(stderr,
            "Expected to read %zu bytes read from '%s' -- actually read %zu.\n",
            fileSize, CL_FILE_NAME, n);

  // create the text for the kernel program:

  char *strings[1];
  strings[0] = clProgramText;
  cl_program program = clCreateProgramWithSource(
      context, 1, (const char **)strings, NULL, &status);
  if (status != CL_SUCCESS)
    fprintf(stderr, "clCreateProgramWithSource failed\n");
  delete[] clProgramText;

  // 8. compile and link the kernel code:

  const char *options = "";
  status = clBuildProgram(program, 1, &device, options, NULL, NULL);
  if (status != CL_SUCCESS) {
    size_t size;
    clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, 0, NULL,
                          &size);
    cl_char *log = new cl_char[size];
    clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, size, log,
                          NULL);
    fprintf(stderr, "clBuildProgram failed:\n%s\n", log);
    delete[] log;
  }

  // 9. create the kernel object:

  cl_kernel kernel = clCreateKernel(program, "ArrayMult", &status);
  if (status != CL_SUCCESS)
    fprintf(stderr, "clCreateKernel failed\n");

  // 10. setup the arguments to the kernel object:

  status = clSetKernelArg(kernel, 0, sizeof(cl_mem), &device_A);
  if (status != CL_SUCCESS)
    fprintf(stderr, "clSetKernelArg failed (1)\n");

  status = clSetKernelArg(kernel, 1, sizeof(cl_mem), &device_B);
  if (status != CL_SUCCESS)
    fprintf(stderr, "clSetKernelArg failed (2)\n");

  status = clSetKernelArg(kernel, 2, sizeof(cl_mem), &device_C);
  if (status != CL_SUCCESS)
    fprintf(stderr, "clSetKernelArg failed (3)\n");

  status = clSetKernelArg(kernel, 3, sizeof(cl_mem), &device_D);
  if (status != CL_SUCCESS)
    fprintf(stderr, "clSetKernelArg failed (4)\n");

  // 11. enqueue the kernel object for execution:

  size_t globalWorkSize[3] = {NUM_ELEMENTS, 1, 1};
  size_t localWorkSize[3] = {LOCAL_SIZE, 1, 1};

  Wait(cmdQueue);
  double time0 = omp_get_wtime();

  time0 = omp_get_wtime();

  status = clEnqueueNDRangeKernel(cmdQueue, kernel, 1, NULL, globalWorkSize,
                                  localWorkSize, 0, NULL, NULL);
  if (status != CL_SUCCESS)
    fprintf(stderr, "clEnqueueNDRangeKernel failed: %d\n", status);

  Wait(cmdQueue);
  double time1 = omp_get_wtime();

  // 12. read the results buffer back from the device to the host:

  status = clEnqueueReadBuffer(cmdQueue, device_C, CL_TRUE, 0, dataSize, host_C,
                               0, NULL, NULL);
  if (status != CL_SUCCESS)
    fprintf(stderr, "clEnqueueReadBuffer failed\n");

#ifndef CSV
  fprintf(stderr, "%8d\t%4d\t%10d\t%10.3lf GigaMultsPerSecond\n", NUM_ELEMENTS,
          LOCAL_SIZE, NUM_WORK_GROUPS,
          (double)NUM_ELEMENTS / (time1 - time0) / 1000000000.);
#else
  printf("%d,%d,%d,%lf\n", NUM_ELEMENTS, LOCAL_SIZE, NUM_WORK_GROUPS,
         (double)NUM_ELEMENTS / (time1 - time0) / 1000000000.);
#endif

  // 13. clean everything up:

  clReleaseKernel(kernel);
  clReleaseProgram(program);
  clReleaseCommandQueue(cmdQueue);
  clReleaseMemObject(device_A);
  clReleaseMemObject(device_B);
  clReleaseMemObject(device_C);
  clReleaseMemObject(device_D);

  delete[] host_A;
  delete[] host_B;
  delete[] host_C;
  delete[] host_D;

  return 0;
}

int LookAtTheBits(float fp) {
  int *ip = (int *)&fp;
  return *ip;
}

// wait until all queued tasks have taken place:

void Wait(cl_command_queue queue) {
  cl_event wait;
  cl_int status;

  status = clEnqueueMarker(queue, &wait);
  if (status != CL_SUCCESS)
    fprintf(stderr, "Wait: clEnqueueMarker failed\n");

  status = clWaitForEvents(1, &wait);
  if (status != CL_SUCCESS)
    fprintf(stderr, "Wait: clWaitForEvents failed\n");
}
