import java.util.List;
import java.util.LinkedList;
import java.util.Vector;

/**
 * Represents the runtime library.
 */
public class LispRTL {

    private static void checkNumberOfArgumentsNotLess(BaseClass [] args, int requiredNumber, String methodName) throws Exception {
        if (args.length < requiredNumber) {
            throw new Exception("Too few arguments for method \"" + methodName + "\"");
        }
    }

    private static void checkNumberOfArgumentsNotMore(BaseClass [] args, int requiredNumber, String methodName) throws Exception {
        if (args.length > requiredNumber) {
            throw new Exception("Too many arguments for method \"" + methodName + "\"");
        }
    }

    private static void checkArgumentType(BaseClass [] args, int argNumber, int [] allowedTypes, String typeStr, String functionName) throws Exception {
        boolean result = false;
        for (int type : allowedTypes) {
            result |= args[argNumber].type == type;
        }
        if (!result) {
            throw new Exception("The argument № " + (argNumber + 1) + " in \"" + functionName + "\" doesn't look like " + typeStr);
        }
    }

    private static boolean hasFloats(BaseClass [] args) {
        for (BaseClass tmp : args) {
            if (tmp.type == BaseClass.TYPE_FLOAT) {
                return true;
            }
        }
        return false;
    }

    private static boolean hasStrings(BaseClass [] args) {
        for (BaseClass tmp : args) {
            if (tmp.type == BaseClass.TYPE_STRING) {
                return true;
            }
        }
        return false;
    }

    public static BaseClass plus(BaseClass [] args) throws Exception {
        checkNumberOfArgumentsNotLess(args, 1, "+");

        BaseClass result = new BaseClass();

        // If there is a string - concatenate everything and return a string.
        boolean hasFloats = hasFloats(args);
        boolean hasStrings = hasStrings(args);

        if (hasStrings) {
            result.type = BaseClass.TYPE_STRING;
        } else if (hasFloats) {
            result.type = BaseClass.TYPE_FLOAT;
        } else {
            result.type = BaseClass.TYPE_INT;
        }

        for (BaseClass tmp : args) {
            if (hasStrings) {
                switch(tmp.type) {
                case BaseClass.TYPE_INT:
                    result.valueString += tmp.valueInt;
                    break;
                case BaseClass.TYPE_FLOAT:
                    result.valueString += tmp.valueFloat;
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
            } else if (hasFloats) {
                result.valueFloat += tmp.valueFloat;
            } else {
                result.valueInt += tmp.valueInt;
            }
        }

        // Update float and int fields if it's a number.
        if (hasFloats) {
            result.valueInt = (int)result.valueFloat;
        } else {
            result.valueFloat = (float)result.valueInt;
        }

        return result;
    }

    public static BaseClass minus(BaseClass [] args) throws Exception {
        checkNumberOfArgumentsNotLess(args, 1, "-");

        BaseClass result = new BaseClass();
        boolean hasFloats = hasFloats(args);
        result.type = hasFloats ? BaseClass.TYPE_FLOAT : BaseClass.TYPE_INT;

        // Some minus-specific magic here.
        if (args.length > 1) {
            if (hasFloats) {
                result.valueFloat = 2 * args[0].valueFloat;
            } else {
                result.valueInt = 2 * args[0].valueInt;
            }
        }
        // Calculate the result.
        for (BaseClass tmp : args) {
            if (hasFloats) {
                result.valueFloat -= tmp.valueFloat;
            } else {
                result.valueInt -= tmp.valueInt;
            }
        }
        // Update float and int fields.
        if (hasFloats) {
            result.valueInt = (int)result.valueFloat;
        } else {
            result.valueFloat = (float)result.valueInt;
        }
        return result;
    }

    public static BaseClass mult(BaseClass [] args) throws Exception {
        checkNumberOfArgumentsNotLess(args, 1, "*");

        BaseClass result = new BaseClass();
        boolean hasFloats = hasFloats(args);
        result.type = hasFloats ? BaseClass.TYPE_FLOAT : BaseClass.TYPE_INT;
        result.valueInt = 1;
        result.valueFloat = 1;
        // Calculate the result.
        for (BaseClass tmp : args) {
            if (hasFloats) {
                result.valueFloat *= tmp.valueFloat;
            } else {
                result.valueInt *= tmp.valueInt;
            }

        }
        // Update float and int fields.
        if (hasFloats) {
            result.valueInt = (int)result.valueFloat;
        } else {
            result.valueFloat = (float)result.valueInt;
        }
        return result;
    }

    public static BaseClass div(BaseClass [] args) throws Exception {
        checkNumberOfArgumentsNotLess(args, 1, "/");

        BaseClass result = new BaseClass();
        result.type = BaseClass.TYPE_INT;
        boolean hasFloats = hasFloats(args);
        result.type = hasFloats ? BaseClass.TYPE_FLOAT : BaseClass.TYPE_INT;
        result.valueInt = 1;
        result.valueFloat = 1;
        // Some div-specific magic here.
        if (args.length == 1) {
            if (hasFloats) {
                result.valueFloat /= args[0].valueFloat;
            } else {
                result.valueInt /= args[0].valueInt;
            }
        } else {
            if (hasFloats) {
                result.valueFloat = args[0].valueFloat;
                for (int i = 1; i < args.length; i++) {
                    result.valueFloat /= args[i].valueFloat;
                }
            } else {
                result.valueInt = args[0].valueInt;
                for (int i = 1; i < args.length; i++) {
                    result.valueInt /= args[i].valueInt;
                }
            }

        }
        // Update float and int fields.
        if (hasFloats) {
            result.valueInt = (int)result.valueFloat;
        } else {
            result.valueFloat = (float)result.valueInt;
        }
        return result;
    }

    public static BaseClass greater(BaseClass [] args) throws Exception {
        checkNumberOfArgumentsNotLess(args, 2, ">");

        BaseClass result = new BaseClass();
        result.type = BaseClass.TYPE_BOOLEAN;
        boolean hasFloats = hasFloats(args);
        boolean res = true;
        for (int i = 0; i < args.length - 1; i++) {
            checkArgumentType(args, i, new int[]{BaseClass.TYPE_INT, BaseClass.TYPE_FLOAT}, "a number", ">");
            checkArgumentType(args, i + 1, new int[]{BaseClass.TYPE_INT, BaseClass.TYPE_FLOAT}, "a number", ">");
            if (hasFloats) {
                res &= args[i].valueFloat > args[i + 1].valueFloat;
            } else {
                res &= args[i].valueInt > args[i + 1].valueInt;
            }
        }
        result.valueBoolean = res ? 1 : 0;
        return result;
    }

    public static BaseClass greater_eq(BaseClass [] args) throws Exception {
        checkNumberOfArgumentsNotLess(args, 2, ">=");

        BaseClass result = new BaseClass();
        result.type = BaseClass.TYPE_BOOLEAN;
        boolean hasFloats = hasFloats(args);
        boolean res = true;
        for (int i = 0; i < args.length - 1; i++) {
            checkArgumentType(args, i, new int[]{BaseClass.TYPE_INT, BaseClass.TYPE_FLOAT}, "a number", ">=");
            checkArgumentType(args, i + 1, new int[]{BaseClass.TYPE_INT, BaseClass.TYPE_FLOAT}, "a number", ">=");
            if (hasFloats) {
                res &= args[i].valueFloat >= args[i + 1].valueFloat;
            } else {
                res &= args[i].valueInt >= args[i + 1].valueInt;
            }
        }
        result.valueBoolean = res ? 1 : 0;
        return result;
    }

    public static BaseClass less(BaseClass [] args) throws Exception {
        checkNumberOfArgumentsNotLess(args, 2, "<");

        BaseClass result = new BaseClass();
        result.type = BaseClass.TYPE_BOOLEAN;
        boolean hasFloats = hasFloats(args);
        boolean res = true;
        for (int i = 0; i < args.length - 1; i++) {
            checkArgumentType(args, i, new int[]{BaseClass.TYPE_INT, BaseClass.TYPE_FLOAT}, "a number", "<");
            checkArgumentType(args, i + 1, new int[]{BaseClass.TYPE_INT, BaseClass.TYPE_FLOAT}, "a number", "<");
            if (hasFloats) {
                res &= args[i].valueFloat < args[i + 1].valueFloat;
            } else {
                res &= args[i].valueInt < args[i + 1].valueInt;
            }
        }
        result.valueBoolean = res ? 1 : 0;
        return result;
    }

    public static BaseClass less_eq(BaseClass [] args) throws Exception {
        checkNumberOfArgumentsNotLess(args, 2, "<=");

        BaseClass result = new BaseClass();
        result.type = BaseClass.TYPE_BOOLEAN;
        boolean hasFloats = hasFloats(args);
        boolean res = true;
        for (int i = 0; i < args.length - 1; i++) {
            checkArgumentType(args, i, new int[]{BaseClass.TYPE_INT, BaseClass.TYPE_FLOAT}, "a number", "<=");
            checkArgumentType(args, i + 1, new int[]{BaseClass.TYPE_INT, BaseClass.TYPE_FLOAT}, "a number", "<=");
            if (hasFloats) {
                res &= args[i].valueFloat <= args[i + 1].valueFloat;
            } else {
                res &= args[i].valueInt <= args[i + 1].valueInt;
            }
        }
        result.valueBoolean = res ? 1 : 0;
        return result;
    }

    public static BaseClass eq(BaseClass [] args) throws Exception {
        checkNumberOfArgumentsNotLess(args, 1, "=");

        BaseClass result = new BaseClass();
        result.type = BaseClass.TYPE_BOOLEAN;
        boolean res = true;
        for (int i = 0; i < args.length - 1; i++) {
            res &= args[i].type == args[i + 1].type;
            switch(args[i].type) {
            case BaseClass.TYPE_INT:
                res &= args[i].valueInt == args[i + 1].valueInt;
                break;
            case BaseClass.TYPE_FLOAT:
                res &= args[i].valueFloat == args[i + 1].valueFloat;
                break;
            case BaseClass.TYPE_CHAR:
                res &= args[i].valueChar == args[i + 1].valueChar;
                break;
            case BaseClass.TYPE_STRING:
                res &= args[i].valueString.equals(args[i + 1].valueString);
                break;
            case BaseClass.TYPE_BOOLEAN:
                res &= args[i].valueBoolean == args[i + 1].valueBoolean;
                break;
            case BaseClass.TYPE_LIST:
                res &= args[i].valueList.equals(args[i + 1].valueList);
                break;
            case BaseClass.TYPE_VECTOR:
                res &= args[i].valueVector.equals(args[i + 1].valueVector);
                break;
            default:
                res = false;
                break;
            }
            if (!res) {
                break;
            }

        }
        result.valueBoolean = res ? 1 : 0;
        return result;
    }

    public static BaseClass and(BaseClass [] args)  throws Exception {
        checkNumberOfArgumentsNotLess(args, 1, "and");

        BaseClass result = new BaseClass();
        result.type = BaseClass.TYPE_BOOLEAN;
        result.valueBoolean = 1;
        for (int i = 0; i < args.length; i++) {
            checkArgumentType(args, i, new int[]{BaseClass.TYPE_BOOLEAN}, "a boolean", "AND");
            if (args[i].valueBoolean == 0) {
                result.valueBoolean = 0;
            }
            if (result.valueBoolean == 0) {
                break;
            }
        }
        return result;
    }

    public static BaseClass or(BaseClass [] args)  throws Exception {
        checkNumberOfArgumentsNotLess(args, 1, "or");

        BaseClass result = new BaseClass();
        result.type = BaseClass.TYPE_BOOLEAN;
        result.valueBoolean = 0;
        for (int i = 0; i < args.length; i++) {
            checkArgumentType(args, i, new int[]{BaseClass.TYPE_BOOLEAN}, "a boolean", "OR");
            if (args[i].valueBoolean == 1) {
                result.valueBoolean = 1;
            }
            if (result.valueBoolean == 1) {
                break;
            }
        }
        return result;
    }

    public static BaseClass not(BaseClass [] args)  throws Exception {
        checkNumberOfArgumentsNotLess(args, 1, "not");
        checkNumberOfArgumentsNotMore(args, 1, "not");

        BaseClass result = new BaseClass();
        result.type = BaseClass.TYPE_BOOLEAN;
        checkArgumentType(args, 0, new int[]{BaseClass.TYPE_BOOLEAN}, "a boolean", "NOT");
        result.valueBoolean = (args[0].valueBoolean != 0) ? 0 : 1;
        return result;
    }

    public static BaseClass setf(BaseClass [] args) {
        // This is a fictive method, actually implemented by the compiler.
        return null;
    }

    public static BaseClass elt(BaseClass [] args) throws Exception {
        checkNumberOfArgumentsNotLess(args, 2, "elt");
        checkArgumentType(args, 0, new int[]{BaseClass.TYPE_LIST, BaseClass.TYPE_VECTOR}, "a container", "ELT");
        checkArgumentType(args, 1, new int[]{BaseClass.TYPE_INT}, "an integer", "ELT");

        List<BaseClass> container = null;
        int index = args[1].valueInt;
        if (args[0].type == BaseClass.TYPE_LIST) {
            container = args[0].valueList;
        } else {
            container = args[0].valueVector;
        }
        if (index < 0 || index >= container.size()) {
            throw new Exception("Index out of bounds in ELT call");
        }

        return container.get(index);
    }

    public static BaseClass setf_elt(BaseClass [] args) throws Exception {
        checkNumberOfArgumentsNotLess(args, 3, "setf elt");
        checkArgumentType(args, 0, new int[]{BaseClass.TYPE_LIST, BaseClass.TYPE_VECTOR}, "a container", "SETF ELT");
        checkArgumentType(args, 1, new int[]{BaseClass.TYPE_INT}, "an integer", "SETF ELT");

        List<BaseClass> container = null;
        int index = args[1].valueInt;
        if (args[0].type == BaseClass.TYPE_LIST) {
            container = args[0].valueList;
        } else {
            container = args[0].valueVector;
        }
        if (index < 0 || index >= container.size()) {
            throw new Exception("Index out of bounds in SETF ELT call");
        }
        container.set(index, args[2]);
        return args[2];
    }

    public static BaseClass list(BaseClass [] args) {
        BaseClass result = new BaseClass();
        result.type = BaseClass.TYPE_LIST;
        result.valueList = new LinkedList<BaseClass>();
        for (BaseClass obj : args) {
            result.valueList.add(obj);
        }
        return result;
    }

    public static BaseClass vector(BaseClass [] args) {
        BaseClass result = new BaseClass();
        result.type = BaseClass.TYPE_VECTOR;
        result.valueVector = new Vector<BaseClass>(args.length);
        for (BaseClass obj : args) {
            result.valueVector.add(obj);
        }
        return result;
    }

    public static BaseClass length(BaseClass [] args) throws Exception {
        checkNumberOfArgumentsNotLess(args, 1, "length");
        checkArgumentType(args, 0, new int[]{BaseClass.TYPE_LIST, BaseClass.TYPE_VECTOR}, "a container", "LENGTH");

        BaseClass result = new BaseClass();
        result.type = BaseClass.TYPE_INT;
        List<BaseClass> container = null;
        if (args[0].type == BaseClass.TYPE_LIST) {
            container = args[0].valueList;
        } else {
            container = args[0].valueVector;
        }
        for (int i = 1; i < args.length; i++) {
            container.add(args[i]);
        }
        result.valueInt = container.size();
        return result;
    }

    public static BaseClass push(BaseClass [] args) throws Exception {
        checkNumberOfArgumentsNotLess(args, 2, "push");
        checkArgumentType(args, 0, new int[]{BaseClass.TYPE_LIST, BaseClass.TYPE_VECTOR}, "a container", "PUSH");

        List<BaseClass> container = null;
        if (args[0].type == BaseClass.TYPE_LIST) {
            container = args[0].valueList;
        } else {
            container = args[0].valueVector;
        }
        for (int i = 1; i < args.length; i++) {
            container.add(args[i]);
        }
        return args[0];
    }

    public static BaseClass pop(BaseClass [] args) throws Exception {
        checkNumberOfArgumentsNotLess(args, 1, "pop");
        checkArgumentType(args, 0, new int[]{BaseClass.TYPE_LIST, BaseClass.TYPE_VECTOR}, "a container", "POP");

        List<BaseClass> container = null;
        if (args[0].type == BaseClass.TYPE_LIST) {
            container = args[0].valueList;
        } else {
            container = args[0].valueVector;
        }

        return container.remove(container.size() - 1) ;
    }

    public static BaseClass print(BaseClass [] args) {
        for (BaseClass obj : args) {
            switch(obj.type) {
            case BaseClass.TYPE_INT:
                System.out.print(obj.valueInt);
                break;
            case BaseClass.TYPE_FLOAT:
                System.out.print(obj.valueFloat);
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
            case BaseClass.TYPE_LIST:
                System.out.print("(");
                int lcnt = 0;
                int lsize = obj.valueList.size();
                for (BaseClass tmp : obj.valueList) {
                    BaseClass[] tmparr = new BaseClass[1];
                    tmparr[0] = tmp;
                    print(tmparr);
                    if (lcnt++ != lsize - 1) {
                        System.out.print(" ");
                    }
                }
                System.out.print(")");
                break;
            case BaseClass.TYPE_VECTOR:
                System.out.print("#(");
                int vcnt = 0;
                int vsize = obj.valueVector.size();
                for (BaseClass tmp : obj.valueVector) {
                    BaseClass[] tmparr = new BaseClass[1];
                    tmparr[0] = tmp;
                    print(tmparr);
                    if (vcnt++ != vsize - 1) {
                        System.out.print(" ");
                    }
                }
                System.out.print(")");
                break;
            default:
                // Don't tell anybody about the error!!!
                break;
            }
        }
        return null;
    }

    public static BaseClass archey() {
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
        BaseClass result = new BaseClass();
        return result;
    }
}
