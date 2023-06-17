/* Python for finding primes. */

// Python headers.
#define PY_SSIZE_T_CLEAN
#include <Python.h>

// C stdlib headers.
#include <stdbool.h>
#include <stdint.h>

#define __version__ "1.1"

// A convenience to convert a C array of integers to a Python list of integers.
static PyObject *ArrayToPyList(uint32_t *arr, uint32_t numEls)
{
    PyObject *list = PyList_New(numEls);
    for (uint32_t idx = 0u; idx < numEls; idx++)
    {
        PyList_SetItem(list, idx, PyLong_FromLong(arr[idx]));
    }
    return list;
}

// Allocate some memory and raise a Python error if it fails (rather than having
// the C just crash and fall over).
static inline void *safe_malloc(size_t size)
{
    void *allocMem = malloc(size);
    // Sometimes implementations return null on a 0 length string, we may as
    // well allow it as it allows more compatibility with very few side effects.
    if (!allocMem && size)
    {
        return PyErr_NoMemory();
    }
    return allocMem;
}

// Return a pointer to a Python list containing the first N primes. Note all
// Python functions take one of two forms, this is for functions with only args
// but if your function takes kwargs it should have a third argument. All
// functions return a PyObject pointer, although it can be None.
static PyObject * FindFirstNPrimes(PyObject *self, PyObject *args)
{
    // Parse the arguments and return NULL if can't parse a number.
    uint32_t target;
    if (!PyArg_ParseTuple(args, "k", &target))
    {
        return NULL;
    }

    // Assign the array of primes that we're going to fill.
    uint32_t *primes = safe_malloc(target * sizeof(uint32_t));
    primes[0u] = 2u; // Got to start somewhere, 2 is the first prime.

    // Calculate all the primes.
    uint32_t idx = 1u;
    uint32_t counter = 1u;  // We add 2 on entering the loop so 3 is checked first.
    while (idx < target)
    {
        counter += 2u;  // Only consider odd numbers.
        bool skip = false;
        for (uint32_t divisorIdx = 0u; divisorIdx < idx; divisorIdx++)
        {
            if (!(counter % primes[divisorIdx]))
            {
                // As soon as counter has a divisor (i.e. no remainder), skip it
                // rather than checking more things for this number.
                skip = true;
            }
        }
        if (!skip)
        {
            primes[idx] = counter;
            idx++;
        }
    }

    // Convert to Python list and deallocate.
    PyObject *primesList = ArrayToPyList(primes, target);
    free(primes);
    return primesList;
}

// Used to add the __doc__ attribute of a module/function/whatever you attach it
// to (in this case the get_primes method). Done like this with a macro because
// you can compile without docstrings to save space, if you want to (but as a
// rule, it's a bad idea).
PyDoc_STRVAR(prime_doc, "Find the first N primes.");

// PyMethodDef defines the methods of this module that you want accessible from Python.
static PyMethodDef CPrimesMethods[] = {

    //       name,             C function to run, type of args, docstring
    {"get_primes", (PyCFunction)FindFirstNPrimes, METH_VARARGS, prime_doc},

    // This NULL line must be present and indicates the end of the array.
    {        NULL,                           NULL,            0,      NULL},

};

// PyModuleDef is the table that gives Python everything it needs to be able to
// initialise your module.
static struct PyModuleDef cprimesmodule = {
    PyModuleDef_HEAD_INIT,  // A required constant used by Python.
    "CPrimes",      // Module name, as a string.
    NULL,           // A docstring object if you want your module to have the
                    // __doc__ attribute (i.e. what you would see if you did
                    // help(CPrimes)).
    -1,             // This is the "size of the per-interpreter state", you
                    // probably don't need to care about it if you're using e.g.
                    // `static` variables for internal state or don't need to be
                    // robust to being called at the same time in several
                    // interpreters at once.
    CPrimesMethods  // The methods table for your module, defined above.
};

// Initialise the module. This is the thing that gets called to create your
// module in memory when it is getting initialised. This is called on the first
// `import CPrimes` encountered in Python.
PyMODINIT_FUNC PyInit_CPrimes(void)
{
    // Actually creates the module object. Do a NULL check in case we failed to
    // initialise it, or some such.
    PyObject *m = PyModule_Create(&cprimesmodule);
    if (m == NULL)
    {
        return NULL;
    }

    // AddStringMacro lets us add top-level string constants to our module. So
    // these lines let us do CPrimes.__version__ or CPrimes.string. Note how the
    // variable name gets used as the attribute name of the module, so these
    // must always be passed as named constants. There also exist things like
    // AddIntMacro etc for other data types.
    PyModule_AddStringMacro(m, __version__);
    const char *string = "Hello my old friend.";
    PyModule_AddStringMacro(m, string);

    // Other things you can do here include creating custom exception types (if
    // you're going to raise them from C), or any import-time side effects.

    return m;
}
