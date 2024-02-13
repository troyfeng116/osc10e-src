package JavaSort;

import java.util.concurrent.RecursiveAction;

public abstract class BaseThreadedSort<T extends Comparable<? super T>> extends RecursiveAction {
    private final T[] theArr;
    private final int theStart;
    private final int theEnd;

    public BaseThreadedSort(T[] aArr, int aStart, int aEnd) {
        theArr = aArr;
        theStart = aStart;
        theEnd = aEnd;
    }

    public T[] getArr() {
        return theArr;
    }

    public int getStart() {
        return theStart;
    }

    public int getEnd() {
        return theEnd;
    }

    protected void swap(int aI, int aJ) {
        T myTmp = theArr[aI];
        theArr[aI] = theArr[aJ];
        theArr[aJ] = myTmp;
    }

    protected static Integer[] generateRandomArr(int aSize) {
        Integer[] myRes = new Integer[aSize];
        for (int myI = 0; myI < aSize; myI++) {
            myRes[myI] = (int) (Math.random() * aSize);
        }
        return myRes;
    }
}
