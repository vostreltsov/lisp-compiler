/**
 * Represents the runtime library.
 */
public class LispRTL {

    // TODO: parameters of functions

    public static BaseClass plus() {
        BaseClass result = new BaseClass();
        return result;
    }

    public static BaseClass minus() {
        BaseClass result = new BaseClass();
        return result;
    }

    public static BaseClass mult() {
        BaseClass result = new BaseClass();
        return result;
    }

    public static BaseClass div() {
        BaseClass result = new BaseClass();
        return result;
    }

    public static BaseClass greater() {
        BaseClass result = new BaseClass();
        return result;
    }

    public static BaseClass greater_eq() {
        BaseClass result = new BaseClass();
        return result;
    }

    public static BaseClass less() {
        BaseClass result = new BaseClass();
        return result;
    }

    public static BaseClass less_eq() {
        BaseClass result = new BaseClass();
        return result;
    }

    public static BaseClass eq() {
        BaseClass result = new BaseClass();
        return result;
    }

    public static BaseClass and() {
        BaseClass result = new BaseClass();
        return result;
    }

    public static BaseClass or() {
        BaseClass result = new BaseClass();
        return result;
    }

    public static BaseClass not() {
        BaseClass result = new BaseClass();
        return result;
    }

    public static void print(BaseClass obj) {
        switch(obj.type) {
        case BaseClass.TYPE_INT:
            System.out.println(obj.valueInt);
            break;
        case BaseClass.TYPE_CHAR:
            System.out.println(obj.valueChar);
            break;
        case BaseClass.TYPE_STRING:
            System.out.println(obj.valueString);
            break;
        case BaseClass.TYPE_ARRAY:
            // TODO.
        break;
        default:
            // Don't tell anybody about the error!!!
            break;
        }
    }
}