package JavaSort;

import java.util.Arrays;
import java.util.concurrent.*;

public class ThreadedQSort<T extends Comparable<? super T>> extends RecursiveAction {
    private static final int SIZE = 2000;
    private static final int THRESHOLD = 100;

    private final T[] theArr;
    private final int theStart;
    private final int theEnd;

    public ThreadedQSort(T[] aArr, int aStart, int aEnd) {
        theArr = aArr;
        theStart = aStart;
        theEnd = aEnd;
    }

    @Override
    protected void compute() {
        int myN = theEnd - theStart;

        if (myN <= THRESHOLD) {
            Arrays.sort(theArr, theStart, theEnd);
            return;
        }

        int myRandIdx = theStart + (int) (Math.random() * myN);
        T myPivot = theArr[myRandIdx];
        swap(myRandIdx, theEnd - 1);
        int myPivotIdx = theStart;
        for (int myI = theStart; myI < theEnd - 1; myI++) {
            if (theArr[myI].compareTo(myPivot) < 0) {
                swap(myI, myPivotIdx);
                myPivotIdx++;
            }
        }

        swap(myPivotIdx, theEnd - 1);

        ThreadedQSort<T> myLeftTask = new ThreadedQSort<>(theArr, theStart, myPivotIdx);
        ThreadedQSort<T> myRightTask = new ThreadedQSort<>(theArr, myPivotIdx + 1, theEnd);

        myLeftTask.fork();
        myRightTask.fork();

        myLeftTask.join();
        myRightTask.join();
        System.out.printf("[ThreadedQSort] (%d, %d) joined (%d, %d) and (%d, %d)\n",
                theStart, theEnd, theStart, myPivotIdx, myPivotIdx + 1, theEnd);
    }

    private void swap(int aI, int aJ) {
        T myTmp = theArr[aI];
        theArr[aI] = theArr[aJ];
        theArr[aJ] = myTmp;
    }

    private static Integer[] generateRandomArr() {
        Integer[] myRes = new Integer[SIZE];
        for (int myI = 0; myI < SIZE; myI++) {
            myRes[myI] = (int) (Math.random() * SIZE);
        }
        return myRes;
    }

    public static void main(String[] args) {
        Integer[] myArr = generateRandomArr();
        ThreadedQSort<Integer> myQSortTask = new ThreadedQSort<>(myArr, 0, SIZE);
        myQSortTask.fork();
        myQSortTask.join();
        System.out.println(Arrays.toString(myArr));
    }
}
