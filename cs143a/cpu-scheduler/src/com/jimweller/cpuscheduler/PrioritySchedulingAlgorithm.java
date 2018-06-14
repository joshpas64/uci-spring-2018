/** PrioritySchedulingAlgorithm.java
 * 
 * A single-queue priority scheduling algorithm.
 *
 * @author: Charles Zhu
 * Spring 2016
 *
 */
package com.jimweller.cpuscheduler;

import java.util.*;

import com.jimweller.cpuscheduler.Process;

public class PrioritySchedulingAlgorithm extends BaseSchedulingAlgorithm implements OptionallyPreemptiveSchedulingAlgorithm {
    private static final int DEFAULT_SIZE = 1;

    //In priority scheduling the highest priority are given to processes with the lowest assigned priority.
    //As a way for the waiting queue to automatically sort the processes according to these criterion,
    //the waiting queue will be a priority with a custom-defined comparator function/algorithm based on Process's priority weight.
    private class PriorityComparator implements Comparator<Process> {
        public int compare(Process p1,Process p2){
            return p1.getPriorityWeight() < p2.getPriorityWeight() ? -1 : p1.getPriorityWeight() != p2.getPriorityWeight() ? 1 : p1.getPID() < p2.getPID() ? -1 : p1.getPID() == p2.getPID() ? 0 : 1;
        }
    }

    private boolean preemptive;
    private PriorityQueue<Process> readyQueue;

    PrioritySchedulingAlgorithm(){
        // Fill in this method
        /*------------------------------------------------------------*/
        this.activeJob = null;
        preemptive = false; //Automatically a non-preemptive algorithm by default.

        //In Java, PriorityQueue's initialized with custom comparators must also be initialized with an initial size.
        //CPU's priority queue will now always be self-sorted by priority weights.
        readyQueue = new PriorityQueue<Process>(DEFAULT_SIZE,new PriorityComparator());

        //We still want to start out with completely blank Priority Queue, we will just pop off the initially blank items 
        //from the Priority Queue that were created with the queue at initialization        
        for(int i = 0; i < DEFAULT_SIZE; i++){
            readyQueue.remove(readyQueue.peek());
        }

        /*------------------------------------------------------------*/
    }

    /** Add the new job to the correct queue.*/
    public void addJob(Process p){
        // Remove the next lines to start your implementation
        //throw new UnsupportedOperationException();
        
        // Fill in this method
        /*------------------------------------------------------------*/
        //Add process to the priority queue of processes.
        //The priority queue's user-defined sorting algorithm will determine its appropriate place in the queue
        //determined by the newly added process's priority weight     
        readyQueue.add(p);

        /*------------------------------------------------------------*/
    }
    
    /** Returns true if the job was present and was removed. */
    public boolean removeJob(Process p){
        // Remove the next lines to start your implementation
        //throw new UnsupportedOperationException();
        
        // Fill in this method
        /*------------------------------------------------------------*/
        //Search for and remove the process from the waiting queue.
        return readyQueue.remove(p);


        /*------------------------------------------------------------*/
    }

    /** Transfer all the jobs in the queue of a SchedulingAlgorithm to another, such as
    when switching to another algorithm in the GUI */
    public void transferJobsTo(SchedulingAlgorithm otherAlg) {
        throw new UnsupportedOperationException();
    }


    /** Returns the next process that should be run by the CPU, null if none available.*/
    public Process getNextJob(long currentTime){
        // Remove the next lines to start your implementation        
        // Fill in this method
        /*------------------------------------------------------------*/
        if(preemptive){
            //In preemptive algorithms, the front item of the queue will always replace and become the
            //currently executing process whether the currently executing process has finished its burst or not.
            //The front item will always be the process that has the lowest priority weight.
            this.activeJob = readyQueue.peek(); 
        } else {
            //In non-preemptive algorithms, the front item of the queue will replace and become the
            //currently executing process only when there is no executing process or if the executing process has already
            //finished its CPU burst.            
            if(this.activeJob == null || this.activeJob.isFinished()){
                this.activeJob = readyQueue.peek();
            }
        }

        return this.activeJob;
        /*------------------------------------------------------------*/
    }

    public String getName(){
        return "Single-Queue Priority";
    }

    /**
     * @return Value of preemptive.
     */
    public boolean isPreemptive(){
        // Remove the next lines to start your implementation
        //throw new UnsupportedOperationException();
        
        // Fill in this method
        /*------------------------------------------------------------*/

        return preemptive;

        /*------------------------------------------------------------*/
    }
    
    /**
     * @param v Value to assign to preemptive.
     */
    public void setPreemptive(boolean v){
        // Remove the next lines to start your implementation
        //throw new UnsupportedOperationException();
        
        // Fill in this method
        /*------------------------------------------------------------*/

        preemptive = v;

        /*------------------------------------------------------------*/
    }
    
}