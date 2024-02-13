package JavaSort;

import java.util.Arrays;

public class ThreadedQSort<T extends Comparable<? super T>> extends BaseThreadedSort<T> {
    private static final int SIZE = 2000;
    private static final int THRESHOLD = 100;

    public ThreadedQSort(T[] aArr, int aStart, int aEnd) {
        super(aArr, aStart, aEnd);
    }

    @Override
    protected void compute() {
        int myN = getEnd() - getStart();

        if (myN <= THRESHOLD) {
            Arrays.sort(getArr(), getStart(), getEnd());
            return;
        }

        int myRandIdx = getStart() + (int) (Math.random() * myN);
        T myPivot = getArr()[myRandIdx];
        swap(myRandIdx, getEnd() - 1);
        int myPivotIdx = getStart();
        for (int myI = getStart(); myI < getEnd() - 1; myI++) {
            if (getArr()[myI].compareTo(myPivot) < 0) {
                swap(myI, myPivotIdx);
                myPivotIdx++;
            }
        }

        swap(myPivotIdx, getEnd() - 1);

        ThreadedQSort<T> myLeftTask = new ThreadedQSort<>(getArr(), getStart(), myPivotIdx);
        ThreadedQSort<T> myRightTask = new ThreadedQSort<>(getArr(), myPivotIdx + 1, getEnd());

        myLeftTask.fork();
        myRightTask.fork();

        myLeftTask.join();
        myRightTask.join();
        System.out.printf("[ThreadedQSort] (%d, %d) joined (%d, %d) and (%d, %d)\n",
                getStart(), getEnd(), getStart(), myPivotIdx, myPivotIdx + 1, getEnd());
    }

    public static void main(String[] args) {
        Integer[] myArr = generateRandomArr(SIZE);
        ThreadedQSort<Integer> myQSortTask = new ThreadedQSort<>(myArr, 0, SIZE);
        myQSortTask.fork();
        myQSortTask.join();
        System.out.println(Arrays.toString(myArr));
    }
}
