#ifndef _TIMESERIES_H_
#define _TIMESERIES_H_
#include <vector>
#include "BasicTimeSeries.h"
#include "dbpp.h"
#include "Metadata.h"
namespace SEISPP {
using namespace std;
using namespace SEISPP;
//@{
// Scalar time series data object.
// This data object extends BasicTimeSeries mainly by adding a vector of
// scalar data.  It uses a Metadata object to contain auxiliary parameters
// that aren't essential to define the data object, but which are necessary
// for some algorithms.  
//@author Gary L. Pavlis
//@}
class TimeSeries: public BasicTimeSeries , public Metadata
{
public:
//@{
// Actual data stored as an STL vector container.  
// Note the STL guarantees the data elements of vector container are 
// contiguous in memory like FORTRAN vectors.  As a result things
// like the BLAS can be used with data object by using a syntax
// like this: if d is a TimeSeries object, the address of the first sample of 
// the data is &(d.s[0]).  
//@}
	vector<double>s;
//@{
// Default constructor.  Initializes object data to zeros and sets the
// initial STL vector size to 0 length.
//@}
	TimeSeries();
//@{
// Similar to the default constructor but creates a vector of data 
// with nsin samples and initializes all samples to 0.0.  
// This vector can safely be accessed with the vector index 
// operator (i.e. operator []).  A corollary is that push_back 
// or push_front applied to this vector will alter it's length
// so use this only if the size of the data to fill the object is
// already known.
//@}
	TimeSeries(int nsin);
//@{
// Partial constructor for a TimeSeries object driven by a Metadata object.
// This is essentially uses the Metadata object as a way to make a parameterized
// constructor with a potentially large and variable number of parameters. 
// The following parameters must exist in the Metadata object or the constructor
// will throw an exception:  samprate, time, and nsamp.  If the load_data
// boolean is true the function will attempt to read data using an additional
// set of keywords that must be in the metadata:  TimeReferenceType, datatype,
// dir, dfile, and foff.  TimeReferenceType is a string that must be either "relative"
// or "absolute".  datatype, dir, dfile, and foff are exactly as in a wfdisc record.
// An important current limitation is that only host 4 byte float datatype (t4 or u4)
// are allowed by this constructor.  If datatype is anything else the constructor will
// throw an exception.  
//
//@throws MetadataError object is thrown if any of the metadata associated with the 
//           keywords noted above are not defined.  
//@throws SeisppError is thrown for read errors when load_data is true.  
//
//@param md - Metadata object to drive construction.
//@param load_data - if true tries to read data in an antelope style ala wfdisc but using
//                     attributes derived from the Metadata object (see above).
//@}
	TimeSeries(Metadata& md,bool load_data);
//@{
// Antelope database driven constructor.
// The basic model here is that this constructor builds a TimeSeries object
// from one row of a database table (normally wfdisc, but the intent is to 
// to be totally general).  
//@param db DatabaseHandle object that is assumed to point at one row of a database view.
//@param mdl A MetadataList contains a list of internal names that tells the 
//   constructor which attributes it will need to extract from the database and place in
//   the Metadata area of the object.  
//@param am This object is used to map internal names to an external namespace.  It is
//  normally created once at the early stage of a program's execution.  
//@}
	TimeSeries(DatabaseHandle& db, MetadataList& mdl, AttributeMap& am);
//@{
// Standard copy constructor.
//@}
	TimeSeries(const TimeSeries&);
//@{
// Standard assignment operator.
//@}
	TimeSeries& operator=(const TimeSeries&);
//@{
// Summation operator.  Simple version of stack.  Aligns data before
// summing.
//@}
	void operator+=(const TimeSeries& d);

//@{
// Scans for defined gaps and sets the data to zero in time periods defined by gaps.
//@}
	void zero_gaps();
};
}  // End SEISPP namespace declaration
#endif
