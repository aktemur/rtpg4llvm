// Used when caller can deal with non-definition
// of a function by trying to use Python.
// Error doesn't need to give method name - the caller always
// knows that - but it should indicate whether the method
// was undefined or multiply-defined.  In the former case,
// the method may be found in the Python defs, but in the
// latter case it would be confusing to simply invoke the
// Python function without informing the user.  (They would
// see the definition in XL and not know why it was being ignored.)
class NodefError extends Exception {

   boolean ismultiplydefined = false;

   public NodefError (boolean ismultiplydefined) {
      this.ismultiplydefined = ismultiplydefined;
   }

   public NodefError () {
   }
}
