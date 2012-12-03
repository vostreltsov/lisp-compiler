import java.util.LinkedList;
import java.util.Vector;
import java.util.Iterator;

/**
 * Represents a base class, all dynamic types inherit this class.
 */
public class BaseClass {
    final public static int TYPE_DAFUQ   = 0;
    final public static int TYPE_INT     = 1;
    final public static int TYPE_CHAR    = 2;
    final public static int TYPE_STRING  = 3;
    final public static int TYPE_BOOLEAN = 4;
    final public static int TYPE_LIST    = 5;
    final public static int TYPE_VECTOR  = 6;

    public int type;
    public int valueInt;
    public char valueChar;
    public String valueString;
    public int valueBoolean;
    public LinkedList<BaseClass> valueList;
    public Vector<BaseClass> valueVector;

    public BaseClass() {
        type = TYPE_DAFUQ;
        valueInt = 0;
        valueChar = '0';
        valueString = "";
        valueBoolean = 0;
        valueList = null;
        valueVector = null;
    }

    public BaseClass(int _int) {
        this();
        type = TYPE_INT;
        valueInt = _int;
    }

    public BaseClass(char _char) {
        this();
        type = TYPE_CHAR;
        valueChar = _char;
    }

    public BaseClass(String _str) {
        this();
        type = TYPE_STRING;
        valueString = _str;
    }

    public BaseClass(int _bool, int _reserved) {
        // _reserved is added to avoid conflicts with the int constructor.
        this();
        type = TYPE_BOOLEAN;
        valueBoolean = _bool;
    }

    public Iterator iterator() {
        if (type == TYPE_LIST) {
            return valueList.iterator();
        } else if (type == TYPE_VECTOR) {
            return valueVector.iterator();
        }
        return null;
    }
}
