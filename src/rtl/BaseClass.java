/**
 * Represents a base class, all dynamic types inherit this class.
 */
public class BaseClass {
    final public static int TYPE_DAFUQ = 0;
    final public static int TYPE_INT = 1;
    final public static int TYPE_CHAR = 2;
    final public static int TYPE_STRING = 3;
    final public static int TYPE_BOOLEAN = 4;
    final public static int TYPE_ARRAY = 5;

    public int type;
    public int valueInt;
    public char valueChar;
    public String valueString;
    public int valueBoolean;
    public BaseClass [] valueArray;

    public BaseClass() {
        type = TYPE_DAFUQ;
        valueInt = 0;
        valueChar = '0';
        valueString = "";
        valueBoolean = 0;
        valueArray = null;
    }
}
