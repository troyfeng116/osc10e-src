package JavaSort;

import java.util.Arrays;

public class ThreadedMSort<T extends Comparable<? super T>> extends BaseThreadedSort<T> {
    private static final int SIZE = 2000;
    private static final int THRESHOLD = 100;

    private final T[] theTmp;

    public ThreadedMSort(T[] aArr, T[] aTmp, int aStart, int aEnd) {
        super(aArr, aStart, aEnd);

        theTmp = aTmp;
    }

    @Override
    protected void compute() {
        int myN = getEnd() - getStart();

        if (myN <= THRESHOLD) {
            Arrays.sort(getArr(), getStart(), getEnd());
            return;
        }

        int myM = getStart() + (getEnd() - getStart()) / 2;

        ThreadedMSort<T> myLeftTask = new ThreadedMSort<>(getArr(), theTmp, getStart(), myM);
        ThreadedMSort<T> myRightTask = new ThreadedMSort<>(getArr(), theTmp, myM, getEnd());

        myLeftTask.fork();
        myRightTask.fork();

        myLeftTask.join();
        myRightTask.join();

        merge(myM);

        System.out.printf("[ThreadedMSort] (%d, %d) joined (%d, %d) and (%d, %d)\n",
                getStart(), getEnd(), getStart(), myM, myM, getEnd());
    }

    private void merge(int aM) {
        int myL = getStart(), myR = aM;
        int myMergedIdx = getStart();
        while (myL < aM && myR < getEnd()) {
            if (getArr()[myL].compareTo(getArr()[myR]) < 0) {
                theTmp[myMergedIdx++] = getArr()[myL++];
            } else {
                theTmp[myMergedIdx++] = getArr()[myR++];
            }
        }

        while (myL < aM) {
            theTmp[myMergedIdx++] = getArr()[myL++];
        }
        while (myR < getEnd()) {
            theTmp[myMergedIdx++] = getArr()[myR++];
        }

        for (int myI = getStart(); myI < getEnd(); myI++) {
            getArr()[myI] = theTmp[myI];
        }
    }

    public static void main(String[] args) {
        Integer[] myArr = generateRandomArr(SIZE);
        ThreadedMSort<Integer> myQSortTask = new ThreadedMSort<>(myArr, new Integer[SIZE], 0, SIZE);
        myQSortTask.fork();
        myQSortTask.join();
        System.out.println(Arrays.toString(myArr));
    }
}
