/**
 * Represents the runtime library.
 */
public class LispRTL {

    public static BaseClass plus(BaseClass [] args) {
        BaseClass result = new BaseClass();
        result.type = BaseClass.TYPE_INT;
        for (BaseClass tmp : args) {
            result.valueInt += tmp.valueInt;
        }
        return result;
    }

    public static BaseClass minus(BaseClass [] args) {
        BaseClass result = new BaseClass();
        result.type = BaseClass.TYPE_INT;
        // Some minus-specific magic here.
        if (args.length > 1 && args[0].valueInt > 0) {
            result.valueInt = 2 * args[0].valueInt;
        }
        for (BaseClass tmp : args) {
            result.valueInt -= tmp.valueInt;
        }
        return result;
    }

    public static BaseClass mult(BaseClass [] args) {
        BaseClass result = new BaseClass();
        return result;
    }

    public static BaseClass div(BaseClass [] args) {
        BaseClass result = new BaseClass();
        return result;
    }

    public static BaseClass greater(BaseClass [] args) {
        BaseClass result = new BaseClass();
        return result;
    }

    public static BaseClass greater_eq(BaseClass [] args) {
        BaseClass result = new BaseClass();
        return result;
    }

    public static BaseClass less(BaseClass [] args) {
        BaseClass result = new BaseClass();
        return result;
    }

    public static BaseClass less_eq(BaseClass [] args) {
        BaseClass result = new BaseClass();
        return result;
    }

    public static BaseClass eq(BaseClass [] args) {
        BaseClass result = new BaseClass();
        return result;
    }

    public static BaseClass and(BaseClass [] args) {
        BaseClass result = new BaseClass();
        return result;
    }

    public static BaseClass or(BaseClass [] args) {
        BaseClass result = new BaseClass();
        return result;
    }

    public static BaseClass not(BaseClass [] args) {
        BaseClass result = new BaseClass();
        return result;
    }

    public static BaseClass setf(BaseClass [] args) {
        BaseClass result = new BaseClass();
        return result;
    }

    public static BaseClass vector(BaseClass [] args) {
        BaseClass result = new BaseClass();
        return result;
    }

    public static BaseClass elt(BaseClass [] args) {
        BaseClass result = new BaseClass();
        return result;
    }

    public static BaseClass list(BaseClass [] args) {
        BaseClass result = new BaseClass();
        return result;
    }

    public static void print(BaseClass [] args) {
        for (BaseClass obj : args) {
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
                print(obj.valueArray);
                break;
            default:
                // Don't tell anybody about the error!!!
                break;
            }
        }        
    }

    public static void archey() {
        System.out.println("               +               "); 
        System.out.println("               #               "); 
        System.out.println("              ###              "); 
        System.out.println("             #####             "); 
        System.out.println("             ######            "); 
        System.out.println("            ; #####;           ");
        System.out.println("           +##.#####           ");
        System.out.println("          +##########          ");
        System.out.println("         #############;        ");
        System.out.println("        ###############+       ");
        System.out.println("       #######   #######       ");
        System.out.println("     .######;     ;######.     ");
        System.out.println("    .#######;     ;#######.    ");
        System.out.println("    #########.   .#########    ");
        System.out.println("   ######'           '######   ");
        System.out.println("  ;####                 ####;  ");
        System.out.println("  ##'                     '##  ");
        System.out.println(" #'                         `# ");
    }
}
