import java.util.LinkedList;
import java.util.Vector;

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
}
