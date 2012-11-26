/**
 * Represents the runtime library.
 */
public class LispRTL {

    public static BaseClass plus(BaseClass [] args) {
        BaseClass result = new BaseClass();

        // If there is a string - concatenate everything and return a string.
        boolean stringExists = false;
        for (BaseClass tmp : args) {
            stringExists |= !tmp.valueString.isEmpty();
        }
        result.type = stringExists ? BaseClass.TYPE_STRING : BaseClass.TYPE_INT;
        for (BaseClass tmp : args) {
            if (stringExists) {
                switch(tmp.type) {
                case BaseClass.TYPE_INT:
                    result.valueString += tmp.valueInt;
                    break;
                case BaseClass.TYPE_CHAR:
                    result.valueString += tmp.valueChar;
                    break;
                case BaseClass.TYPE_STRING:
                    result.valueString += tmp.valueString;
                    break;
                case BaseClass.TYPE_BOOLEAN:
                    result.valueString += tmp.valueBoolean != 0 ? "T" : "NIL";
                    break;
                default:
                    result.valueString += tmp.toString();
                    break;
                }

            } else {
                result.valueInt += tmp.valueInt;
            }
        }
        return result;
    }

    public static BaseClass minus(BaseClass [] args) {
        BaseClass result = new BaseClass();
        result.type = BaseClass.TYPE_INT;
        // Some minus-specific magic here.
        if (args.length > 1) {
            result.valueInt = 2 * args[0].valueInt;
        }
        for (BaseClass tmp : args) {
            result.valueInt -= tmp.valueInt;
        }
        return result;
    }

    public static BaseClass mult(BaseClass [] args) {
        BaseClass result = new BaseClass();
        result.type = BaseClass.TYPE_INT;
        result.valueInt = 1;
        for (BaseClass tmp : args) {
            result.valueInt *= tmp.valueInt;
        }
        return result;
    }

    public static BaseClass div(BaseClass [] args) throws Exception {
        if (args.length == 0) {
            throw new Exception("Too few arguments for division");
        }
        BaseClass result = new BaseClass();
        result.type = BaseClass.TYPE_INT;
        result.valueInt = 1;
        // Some div-specific magic here.
        if (args.length == 1) {
            if (args[0].valueInt == 0) {
                throw new Exception("Division by zero");
            }
            result.valueInt /= args[0].valueInt;
        } else {
            result.valueInt = args[0].valueInt;
            for (int i = 1; i < args.length; i++) {
                if (args[i].valueInt == 0) {
                    throw new Exception("Division by zero");
                }
                result.valueInt /= args[i].valueInt;
            }
        }
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
                System.out.print(obj.valueInt);
                break;
            case BaseClass.TYPE_CHAR:
                System.out.print(obj.valueChar);
                break;
            case BaseClass.TYPE_STRING:
                System.out.print(obj.valueString);
                break;
            case BaseClass.TYPE_BOOLEAN:
                System.out.print(obj.valueBoolean != 0 ? "T" : "NIL");
                break;
            case BaseClass.TYPE_ARRAY:
                System.out.print("[");
                for (BaseClass tmp : obj.valueArray) {
                    BaseClass[] tmparr = new BaseClass[1];
                    tmparr[0] = tmp;
                    print(tmparr);
                    if (tmp != obj.valueArray[obj.valueArray.length]) {
                        System.out.print(" ");
                    }
                }
                System.out.print("]");
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
