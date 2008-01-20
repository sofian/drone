package drone.util;

public class Pair<F extends Comparable, S extends Comparable> implements Comparable {

	public Pair(F first, S second) {
		this.first = first;
		this.second = second;
	}
	
	public boolean equals(Object obj) {
		return (compareTo(obj) == 0);
	}
	
	public int compareTo(Object obj) {
		if (this == obj)
			return 0;
		
		if (obj instanceof Pair) {
			Pair p = (Pair)obj;
			int firstCompare = p.first.compareTo(first);
			int secondCompareAbs = Math.abs(p.second.compareTo(second));
			if (firstCompare < 0)
				return firstCompare - secondCompareAbs;
			else
				return firstCompare + secondCompareAbs;
		} else {
			return Integer.MAX_VALUE;
		}
	}

	public F first;
	public S second;	
	
}
