# FCA checker

## Overview

This script aims at checking the state of the Fabric Collective Accelerator (FCA) service.

To use it just type:

     > fca-health-checker

Example of output:

     Local infiniband IP is: 192.168.1.12
     Testing FCA telnet CLI ...
     FCA telnet CLI is up
     Testing FCA FMM ...
     FMM found ah=0x9fce30

     FMM replied from [FMM `FMM::aradmin1', lid 5, 0x2899f14fd15529a8], GUID: 0x248a07030065176a

     FCA FMM is up
     Benchmark client still running, killing... 
     Benchmark client still running, killing... 
     Benchmark client has terminated
     REDUCE_TEST: Got communicator, 48 bytes from rank0

     1505216400.863467 [FCA_CORE aradmin1 15560] fca_proto.c:498 warn  Communicator 8 does not exist - ignoring COMM_READY (msg_id: 0)

     Benchmark server still running, killing... 
     Benchmark server still running, killing... 
     Benchmark server has terminated
     REDUCE_TEST: Waiting for 1 connections..

     REDUCE_TEST: got info of 20 bytes from rank  #0 (total: 1)

     REDUCE_TEST: Creating new communicator with 2 node roots...

     REDUCE_TEST: Got communicator

     Benchmark created a FCA comunicator succesfully
     Tests results: {'fca_bench': 0, 'fca_cli': 0, 'fca_fmm': 0}

It will return 0 if the service is up and responsive, 1 otherwise.

      > echo $?
      0

Three paramters are set by defualt:
>>>>>>> master
- FCA installation path: */opt/mellanox/fca*
- FCA Reduce test installation path: */opt/mellanox/fca/bin/reduce_test*
- Local infiniband IP, by default is retrieved using ib0 ip address

## How does it work?

This script performs three tests in order to check the state of the FCA service:
 - It first try to contact the CLI using the 2345 port
 - It uses the binary *fca_find_fmm* provided by Mellanox to verify the presence of FMM service
 - It tries to initialize a collective communication using a benchmark provided by Mellanox
 

## REDUCE TEST

PATH_REDUCE_TEST has to be remplaced by the path of the reduce test binary provided by mellanox,
normally it is located under *Mellanox_installation_path/fca/sdk/examples/fca1/*. In some cases this binary has to be compiled. 

## Behaviour of MPI application when FCA service is down

To run an MPI application using FCA support, we type the following commands:

     export OMPI_PARAMS="-mca btl_openib_if_include mlx5_0:1 -mca btl sm,self,openib"
     export FCA_PARAMS="-mca coll_fca_np 0 -mca coll_fca_enable 1"
     mpirun -np $NUM_OF_PROCS $OMPI_PARAMS $FCA_PARAMS --machinefile machine_file --map-by node ./appli

There are two possible behaviours if the FCA is down:

### Timeout


     [node2:52488] Error: coll_fca_module.c:188 - __fca_comm_new() COMM_NEW failed: Connection timed out
     [node2:52492] Error: coll_fca_module.c:188 - __fca_comm_new() COMM_NEW failed: Connection timed out


     1502464532.984624 [FCA_CORE node1 46436] fca_proto.c:37 error Resending exhausted in protocol 'COMM_NEW'
     1502464532.984642 [FCA_CORE node1 46436] fca_proto.c:42 error LID 8 QPN 0x00006483 GUID 0x248a07130062a600 postrecv num=1023, rx completions=0
     1502464532.984668 [FCA_CORE node1 46436] fca_proto.c:102 error COMM_NEW ended after 200.200 seconds, status: Connection timed out
     1502464532.984676 [FCA_CORE node1 46436] fca_proto.c:213 error Failed for job_guid 0x3518363817ead9c5 msg_id 1020751745

### FMM not found

This happend when the service FCA has been stoped during a considerable amount of time:

     1504535969.024918 [FCA_CORE node1 42080] fca_proto.c:664 error Service record 'fca' not found
     1504535969.024961 [FCA_CORE node1 42080] fca_proto.c:748 error Failed to find FMM service record: No such file or directory
     1504535969.024976 [FCA_CORE node1 42080] fca_proto.c:197 error Failed to find FMM
     [arcn1:42080] Error: coll_fca_module.c:188 - __fca_comm_new() COMM_NEW failed: FMM not found
     [arcn1:42081] Error: coll_fca_module.c:188 - __fca_comm_new() COMM_NEW failed: FMM not found
     [arcn1:42082] Error: coll_fca_module.c:188 - __fca_comm_new() COMM_NEW failed: FMM not found


