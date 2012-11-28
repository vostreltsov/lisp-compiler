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

    public static BaseClass plus(BaseClass [] args) throws Exception {
        checkNumberOfArgumentsNotLess(args, 1, "+");

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

    public static BaseClass minus(BaseClass [] args) throws Exception {
        checkNumberOfArgumentsNotLess(args, 1, "-");

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

    public static BaseClass mult(BaseClass [] args) throws Exception {
        checkNumberOfArgumentsNotLess(args, 1, "*");

        BaseClass result = new BaseClass();
        result.type = BaseClass.TYPE_INT;
        result.valueInt = 1;
        for (BaseClass tmp : args) {
            result.valueInt *= tmp.valueInt;
        }
        return result;
    }

    public static BaseClass div(BaseClass [] args) throws Exception {
        checkNumberOfArgumentsNotLess(args, 1, "/");

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

    public static BaseClass greater(BaseClass [] args) throws Exception {
        checkNumberOfArgumentsNotLess(args, 2, ">");

        BaseClass result = new BaseClass();
        result.type = BaseClass.TYPE_BOOLEAN;
        boolean res = true;
        for (int i = 0; i < args.length - 1; i++) {
            if (args[i].type != BaseClass.TYPE_INT) {
                throw new Exception("The operand № " + (i + 1) + " in \">\" call doesn't look like an integer");
            }
            if (args[i + 1].type != BaseClass.TYPE_INT) {
                throw new Exception("The operand № " + (i + 2) + " in \">\" call doesn't look like an integer");
            }
            res &= args[i].valueInt > args[i + 1].valueInt;

        }
        result.valueBoolean = res ? 1 : 0;
        return result;
    }

    public static BaseClass greater_eq(BaseClass [] args) throws Exception {
        checkNumberOfArgumentsNotLess(args, 2, ">=");

        BaseClass result = new BaseClass();
        result.type = BaseClass.TYPE_BOOLEAN;
        boolean res = true;
        for (int i = 0; i < args.length - 1; i++) {
            if (args[i].type != BaseClass.TYPE_INT) {
                throw new Exception("The operand № " + (i + 1) + " in \">=\" call doesn't look like an integer");
            }
            if (args[i + 1].type != BaseClass.TYPE_INT) {
                throw new Exception("The operand № " + (i + 2) + " in \">=\" call doesn't look like an integer");
            }
            res &= args[i].valueInt >= args[i + 1].valueInt;

        }
        result.valueBoolean = res ? 1 : 0;
        return result;
    }

    public static BaseClass less(BaseClass [] args) throws Exception {
        checkNumberOfArgumentsNotLess(args, 2, "<");

        BaseClass result = new BaseClass();
        result.type = BaseClass.TYPE_BOOLEAN;
        boolean res = true;
        for (int i = 0; i < args.length - 1; i++) {
            if (args[i].type != BaseClass.TYPE_INT) {
                throw new Exception("The operand № " + (i + 1) + " in \"<\" call doesn't look like an integer");
            }
            if (args[i + 1].type != BaseClass.TYPE_INT) {
                throw new Exception("The operand № " + (i + 2) + " in \"<\" call doesn't look like an integer");
            }
            res &= args[i].valueInt < args[i + 1].valueInt;

        }
        result.valueBoolean = res ? 1 : 0;
        return result;
    }

    public static BaseClass less_eq(BaseClass [] args) throws Exception {
        checkNumberOfArgumentsNotLess(args, 2, "<=");

        BaseClass result = new BaseClass();
        result.type = BaseClass.TYPE_BOOLEAN;
        boolean res = true;
        for (int i = 0; i < args.length - 1; i++) {
            if (args[i].type != BaseClass.TYPE_INT) {
                throw new Exception("The operand № " + (i + 1) + " in \"<=\" call doesn't look like an integer");
            }
            if (args[i + 1].type != BaseClass.TYPE_INT) {
                throw new Exception("The operand № " + (i + 2) + " in \"<=\" call doesn't look like an integer");
            }
            res &= args[i].valueInt <= args[i + 1].valueInt;

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

        }
        result.valueBoolean = res ? 1 : 0;
        return result;
    }

    public static BaseClass and(BaseClass [] args)  throws Exception {
        checkNumberOfArgumentsNotLess(args, 1, "and");

        BaseClass result = new BaseClass();
        result.type = BaseClass.TYPE_BOOLEAN;
        result.valueBoolean = 1;
        for (BaseClass tmp : args) {
            if (tmp.type != BaseClass.TYPE_BOOLEAN) {
                throw new Exception("The operand in AND call doesn't look like a boolean");
            }
            if (tmp.valueBoolean == 0) {
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
        for (BaseClass tmp : args) {
            if (tmp.type != BaseClass.TYPE_BOOLEAN) {
                throw new Exception("The operand in AND call doesn't look like a boolean");
            }
            if (tmp.valueBoolean == 1) {
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
        if (args[0].type != BaseClass.TYPE_BOOLEAN) {
            throw new Exception("The operand in NOT call doesn't look like a boolean");
        }
        result.valueBoolean = (args[0].valueBoolean != 0) ? 0 : 1;
        return result;
    }

    public static BaseClass setf(BaseClass [] args) {
        // This is a fictive method, actually implemented by the compiler.
        return null;
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

    public static BaseClass elt(BaseClass [] args) throws Exception {
        checkNumberOfArgumentsNotLess(args, 2, "elt");

        if (args[1].type != BaseClass.TYPE_INT) {
            throw new Exception("The second operand in ELT call doesn't look like an integer");
        }

        List<BaseClass> container = null;
        int index = args[1].valueInt;
        switch (args[0].type) {
        case BaseClass.TYPE_LIST:
            container = args[0].valueList;
            break;
        case BaseClass.TYPE_VECTOR:
            container = args[0].valueVector;
            break;
        default:
            throw new Exception("The first operand in ELT call doesn't look like a container");
        }
        if (index < container.size()) {
            return container.get(index);
        } else {
            throw new Exception("Index out of bounds in ELT call");
        }
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

    public static BaseClass print(BaseClass [] args) {
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
            case BaseClass.TYPE_LIST:
                System.out.print("(");
                for (BaseClass tmp : obj.valueList) {
                    BaseClass[] tmparr = new BaseClass[1];
                    tmparr[0] = tmp;
                    print(tmparr);
                    if (tmp != obj.valueList.getLast()) {
                        System.out.print(" ");
                    }
                }
                System.out.print(")");
                break;
            case BaseClass.TYPE_VECTOR:
                System.out.print("#(");
                for (BaseClass tmp : obj.valueVector) {
                    BaseClass[] tmparr = new BaseClass[1];
                    tmparr[0] = tmp;
                    print(tmparr);
                    if (tmp != obj.valueVector.lastElement()) {
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
        BaseClass result = new BaseClass();
        return result;
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
