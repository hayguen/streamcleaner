/*
	 Copyright 2023 Joshuah Rainstar
1.	The source code provided here (hereafter, "the software"), is copyrighted by Joshuah Rainstar (hereafter, “Author”)
	and ownership of all right, title and interest in and to the Software remains with Author. 
2.	The definition of "Use" shall include copying, redistributing, modifying, compiling, interpreting, translating, making derivatives in other programming
	languages, making emulations based on measuring the capability of, and running or executing anything derived from or based on "The Software".
3.	By using or copying the Software, you (hereafter, "User") agree to abide by the terms of this Agreement.
4.	By the grace of God, and the merciful love of the Savior, the Lord Jesus Christ, the software is free as in free beer.
5.	The Users agree that "the software" shall include any part of this code not on 2/19/23 part of either the c++ language specification or standard library.
6.	Author grants to User a royalty-free, nonexclusive right to use the software for academic, research and personal uses,subject to the following conditions:
7:      You may not use the software for any purpose without including this license. 
8.	Any binaries or derivative products produced by the compilation, translation, or emulation, with any tool, including AI, of the Software,
	shall be subject to the same Agreement, and shall remain copywrited by the Author.
9.	User acknowledges that the Software is supplied "as is," without any absolute guarantee of support services from Author.
10.	Author makes no representations or warranties, express or implied, including, without limitation, 
	any representations or warranties of the merchantability or fitness for any particular purpose, 
	or that the application of the software, will not infringe on any patents or other proprietary rights of others.
11.	Author shall not be held liable for direct, indirect, incidental or consequential damages arising from any claim by User
	or any third party with respect to uses allowed under the Agreement, or from any use of the Software.
12.	User agrees to fully indemnify and hold harmless Author from and against any and all claims, demands, suits, losses, damages,
	costs and expenses arising out of the User's use of the Software, including, without limitation, arising out of the User's modification of the Software.
13.	User may modify the Software and distribute that modified work to third parties provided that: (a) if posted separately,
	it clearly acknowledges that it contains material copyrighted by the Author (b) User agrees to notify he Author of the distribution, 
	and (c) User clearly notifies secondary users that such modified work is not the original Software by including the License Agreement.
14.	The Agreement will terminate immediately upon User's breach of, or non-compliance with, any of its terms. 
	User may be held liable for any copyright infringement or the infringement of any other proprietary rights in the Software 
	that is caused or facilitated by the User's failure to abide by the terms of the Agreement.
15.	The Agreement will be construed and enforced in accordance with the law of the state of Colorado.  
	The parties irrevocably consent to the exclusive jurisdiction of the state or federal courts located in Denver for all disputes concerning this agreement.
16.	User agrees not to include, bundle, integrate, or redistribute the software as part of any package, project, program or suite which 
        is sold, leased, rented, or otherwise charged for. 		
*/

//please note: this project is still a work in process and is not finished.
//please do not attempt to use this code for any purpose until this line is removed.

#include <iostream>
#include <vector>
#include <complex>
#include <numeric>
#include <algorithm>
#include <cmath>
#include <array>
# define M_PI           3.14159265358979323846  /* pi */

using namespace std;

//COMPILES
double man(vector<double> arr) {
	// Filter out zeros and NaNs
	vector<double> non_zero_arr(arr.size());
	auto non_zero_end = copy_if(arr.begin(), arr.end(), non_zero_arr.begin(),
		[](double d) { return (d != 0.0) && !isnan(d); });
	non_zero_arr.erase(non_zero_end, non_zero_arr.end());

	// Calculate median and median absolute deviation
	if (non_zero_arr.empty()) {
		return 0.0;//TODO: replace with correct eps value
	}

	double med = non_zero_arr[non_zero_arr.size() / 2];
	vector<double> abs_diff(non_zero_arr.size());
	transform(non_zero_arr.begin(), non_zero_arr.end(), abs_diff.begin(),
		[med](double d) { return abs(d - med); });
	sort(abs_diff.begin(), abs_diff.end());
	double mad = abs_diff[abs_diff.size() / 2];

	return mad;
}

//COMPILES
double atd(vector<double> arr) {
	double med = man(arr);

	// Calculate the absolute deviation from the median
	vector<double> abs_diff(arr.size());
	transform(arr.begin(), arr.end(), abs_diff.begin(),
		[med](double d) { return abs(d - med); });

	// Calculate the average of the squared absolute deviations
	auto is_not_nan = [](double d) { return !isnan(d); };
	auto count = count_if(abs_diff.begin(), abs_diff.end(), is_not_nan);
	double sum_squares = accumulate(abs_diff.begin(), abs_diff.end(), 0.0,
		[](double a, double b) { return a + pow(b, 2); });
	double mean_squares = sum_squares / count;
	return sqrt(mean_squares);
}

//COMPILES
double threshold(vector<double> data) {
	double ata = atd(data);

	vector<double> non_nan_data(data.size());
	auto non_nan_end = copy_if(data.begin(), data.end(), non_nan_data.begin(),
		[](double d) { return !isnan(d) && (d != 0.0); });
	non_nan_data.erase(non_nan_end, non_nan_data.end());

	// Compute the median of non-zero and non-NaN data
	if (non_nan_data.empty()) {
		return ata;
	}
	sort(non_nan_data.begin(), non_nan_data.end());
	double median = non_nan_data[non_nan_data.size() / 2];

	return ata + median;
}



//compiles - contains fatal assertion from innerproduct
vector<double> same_convolution_double_1d(vector<double> ap1, vector<double> ap2) {
	int n1 = ap1.size();
	int n2 = ap2.size();

	int n_left = n2 / 2;
	int n_right = n2 - n_left - 1;

	vector<double> ret;
	ret.resize(n1);

	int idx = 0;

	for (int i = n2 - n_left; i < n2; ++i) {
		ret[idx] = std::inner_product(ap1.begin(), ap1.begin() + i, ap2.end() - i, 0.0);
		idx += 1;
	}

	for (int i = 0; i < n1 - n2 + 1; ++i) {
		ret[idx] = std::inner_product(ap1.begin(), ap1.begin() + i + n2, ap2.end() - i, 0.0);
		idx += 1;
	}

	for (int i = n2 - 1; i >= n2 - n_right; --i) {
		ret[idx] = std::inner_product(ap1.begin() - 1, ap1.begin(), ap2.end() - i, 0.0);
		idx += 1;
	}

	return ret;
}

//COMPILES
vector<double> double_zero_pad_1d(vector<double> array, int pad_width) {
	int size = array.size();
	vector<double> padded;
	padded.resize(size + pad_width * 2);

	fill(padded.begin(), padded.end(), 0.0);
	copy(array.begin(), array.end(), padded.begin() + pad_width);

	return padded;
}

//compiles
vector<double> moving_average(vector<double> x, int w) {
	int n = x.size();
	vector<double> weights(w, 1.0);
	vector<double> sums = same_convolution_double_1d(x, weights);

	double weight_double = static_cast<double>(w);
	for (double sum : sums) {
		sum /= weight_double;
	}

	return sums;
}

//compiles


//compiles
vector<vector<double>> sawtooth_filter(vector<vector<double>> data) {
	// Define filter
	vector<double> filter = { 0.,0.14285714,0.28571429,0.42857143,0.57142857,0.71428571,0.85714286,1.,0.85714286,0.71428571,0.57142857,0.42857143,0.28571429,0.14285714,0. };

	// Zero-pad data
	int pad = 15;

	int rows = data.size();
	int cols = data[0].size();
	int size = rows * (cols + 2 * pad);
	vector<double> flat_data(size, 0.0);

	int k = 0;
	for (int i = 0; i < rows; ++i) {
		k = (i * (cols + 2 * pad)) - pad;
		copy(data[i].begin(), data[i].end(), flat_data.begin() + k);
	}

	vector<double> filtered_data = same_convolution_double_1d(flat_data, filter);

	// Extract result and scale

	vector<vector<double>> result(data.size(), vector<double>(data[0].size()));
	k = 0;
	for (int i = 0; i < data.size(); i++) {
		k = (i * (cols + 2 * pad)) - pad;
		for (int j = pad; j < data[0].size() + pad; j++) {
			result[i][j] = filtered_data[k++] / 7;
		}
	}

	return result;
}


//compiles
vector<vector<double>> transpose(vector<vector<double>> v) {
	int rows = v.size();
	int cols = v[0].size();

	// Create transposed matrix with proper size
	vector<vector<double>> result(cols, vector<double>(rows));

	// Transpose matrix
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			result[j][i] = v[i][j];
		}
	}

	return result;
}


//compiles
vector<vector<double>> convolve_custom_filter_2d(vector<vector<double>> data, int N, int M, int O) {
	// Pad data
	int E = N * 2;
	int F = M * 2;
	vector<vector<double>> padded(data.size() + E * 2, vector<double>(data[0].size() + F * 2, 0.0));
	for (int i = 0; i < data.size(); i++) {
		for (int j = 0; j < data[0].size(); j++) {
			padded[i + E][j + F] = data[i][j];
		}
	}
	vector<vector<double>> padded_t(data[0].size() + F * 2, vector<double>(data.size() + E * 2, 0.0));
	for (int i = 0; i < data.size(); i++) {
		for (int j = 0; j < data[0].size(); j++) {
			padded_t[i + F][j + E] = data[j][i];
		}
	}

	for (int i = 0; i < O; i++) {
		// Flatten matrices
		vector<double> flat_normal(padded.size() * padded[0].size());
		vector<double> flat_normal_t(padded.size() * padded[0].size());
		int k = 0;
		for (int j = 0; j < padded.size(); j++) {
			for (int l = 0; l < padded[j].size(); l++) {
				flat_normal[k] = padded[j][l];
				flat_normal_t[k] = padded_t[l][j];
				k++;
			}
		}

		// Convolve with filter
		vector<double> conv_b = same_convolution_double_1d(flat_normal, vector<double>(N, 1.0));
		vector<double> conv_c = same_convolution_double_1d(flat_normal_t, vector<double>(M, 1.0));

		// Scale convolved data
		for (int j = 0; j < conv_b.size(); j++) {
			conv_b[j] /= (double)N;
			conv_c[j] /= (double)M;
		}

		// Reshape convolved data
		k = 0;
		for (int j = 0; j < padded.size(); j++) {
			for (int l = 0; l < padded[j].size(); l++) {
				padded[j][l] = conv_b[k];
				padded_t[l][j] = conv_c[k];
				k++;
			}
		}

		// Average normal and transpose
		for (int j = 0; j < padded.size(); j++) {
			for (int l = 0; l < padded[j].size(); l++) {
				padded[j][l] = (padded[j][l] + padded_t[l][j]) / 2.0;
			}
		}
	}

	// Extract result
	int pad = F - 1;
	vector<vector<double>> result(padded.size() - E, vector<double>(padded[0].size() - pad));
	for (int i = E; i < padded.size(); i++) {
		for (int j = F - 1; j < padded[0].size() - F + 1; j++) {
			result[i - E][j - F + 1] = padded[i][j];
		}
	}
	return result;
}


// note: always interpolates to between 0 and 1, from a sorted array.
inline void linear_size_interpolation(array<double, 37>& d) {
	double dx = d[36] - d[0];
	for (int i = 0; i < 37; i++) {
		d[i] = (d[i] - d[0]) / dx;
	}
}


//37 bins

//compiles

//compiles
//inlines 1d interpolation and corrceoff directly into the function
void fast_entropy(array<array<double, 37>, 192>& data, array<double, 192>& entropy_unmasked, const array<double, 512>& logit) {
	for (int i = 0; i < data.size(); i++) {
		array<double, 37>& d = data[i];
		sort(d.begin(), d.end());
		double dx = d.back() - d.front();
		for (int j = 0; j < d.size(); j++) {
			d[j] = (d[j] - d.front()) / dx;
		}
		double sum_X = accumulate(d.begin(), d.end(), 0.0);
		double sum_Y = accumulate(logit.begin(), logit.end(), 0.0);
		double sum_XY = inner_product(d.begin(), d.end(), logit.begin(), 0.0);
		double squareSum_X = inner_product(d.begin(), d.end(), d.begin(), 0.0);
		double squareSum_Y = inner_product(logit.begin(), logit.end(), logit.begin(), 0.0);
		entropy_unmasked[i] = 1.0 - (data.size() * sum_XY - sum_X * sum_Y) /
			sqrt((data.size() * squareSum_X - sum_X * sum_X) *
				(data.size() * squareSum_Y - sum_Y * sum_Y));
	}
}

//compiles
double determine_entropy_maximum(int size, vector<double> logit) {
	vector<double> d(size, 0.0);
	d[size - 1] = 1.0;
	return 1.0 - correlationCoefficient(d, logit);
}

//compiles
vector<vector<int>> fast_peaks(vector<vector<double>> real_, vector<int> entropy, double thresh, vector<double> entropy_unmasked, double entropy_maximum)
{
	vector<vector<int>> mask(real_.size(), vector<int>(real_[0].size()));

	for (int each = 0; each < real_[0].size(); each++)
	{
		if (entropy[each] == 0)
		{
			continue; //skip the calculations for this row, it's masked already
		}

		double constant = atd(real_[each]) + man(real_[each]);

		double test = entropy_unmasked[each] / entropy_maximum;
		test = abs(test - 1);

		double thresh1 = (thresh * test);
		if (isnan(thresh1))
		{
			thresh1 = constant; //catch errors
		}

		constant = (thresh1 + constant) / 2;

		for (int i = 0; i < real_[0].size(); i++)
		{
			if (real_[i][each] > constant)
			{
				mask[i][each] = 1;
			}
		}
	}
	return mask;
}

//compiles
int longestConsecutive(vector<int> nums) {
	int curr_streak = 0;
	int max_streak = 0;
	for (int num : nums) {
		if (num == 1) {
			curr_streak++;
		}
		else if (num == 0) {
			max_streak = max(max_streak, curr_streak);
			curr_streak = 0;
		}
	}
	return max(max_streak, curr_streak);
}

//compiles
void remove_outliers(array<int, 192>& a, const int& value, const int& threshold, const int& replace) {
	int first = 0;
	int end = 0;
	int index = 0;
	while (first < a.size()) {
		if (a[first] == value) {
			index = first;
			while (index < a.size() && a[index] == value) {
				index += 1;
			}
			end = index;
			if (end - first + 1 < threshold) {
				for (int i = first; i < end; i++) {
					a[i] = replace;
				}
			}
			first = end;
		}
		else {
			index = first;
			while (index < a.size() && a[index] != value) {
				index += 1;
			}
			first = index;
		}
	}
}

//compiles
void rfft(array<double, 257>& x, array<complex<double>, 257> & X) {
	const int N = 257;

	// Compute the FFT of the positive frequencies
	for (int k = 0; k < N / 2; k++) {
		double real = 0;
		double imag = 0;

		// Loop over each sample index n in the input array x
		for (int n = 0; n < N; n++) {
			// Compute the real and imaginary components of X[k]
			double angle = 2 * M_PI * k * n / N;
			real += x[n] * cos(angle);
			imag -= x[n] * sin(angle);
		}

		// Store the computed real and imaginary components in X[k]
		X[k] = complex<double>(real, imag);
	}

	// Compute the FFT of the negative frequencies by conjugating X[N/2 - k]
	for (int k = 1; k < N / 2; k++) {
		X[N / 2 - k] = conj(X[k]);
	}

	X[N / 2] = -conj(X[N / 2 - 1]);
}


//compiles
static void irfft_fftshift_1d(array<complex<double>, 257>& X, array<double, 257>& temp) {
	//note: this combines a simple inverse fft with a fftshift.
	int N = 257;

	// Compute the real part of the IFFT
	for (int n = 0; n < N; n++) {
		double real_part = 0;
		for (int k = 0; k < N; k++) {
			double angle = 2 * M_PI * k * n / N;
			real_part += X[k].real() * cos(angle) - X[k].imag() * sin(angle);
		}
		temp[n] = real_part / N;
	}

	// Perform FFT shift
	int pivot = (N % 2 == 0) ? (N / 2) : ((N - 1) / 2);
	int right_half = N - pivot;
	int left_half = pivot;

	for (int i = 0; i < right_half; i++) {
		swap(temp[i], temp[i + left_half]);
	}
}

//compiles
void stft( array<double, 25087>& xp, const array<double, 512>& window,  array<array<double, 257>, 192>& Sx,  array<array<complex<double>, 257>, 192>& out){
	const int n_fft = 512;
	const int win_length = 512;
	const int N = 8192;
	const int seg_len = n_fft;
	const int n_overlap = n_fft - 128;
	const int hop_len = seg_len - n_overlap;
	const int n_segs = (static_cast<int>(xp.size()) - seg_len) / hop_len + 1;
	const int s20 = static_cast<int>(ceil(seg_len / 2.0));
	const int s21 = (seg_len % 2 == 1) ? s20 - 1 : s20;
	for (int i = 0; i < n_segs; i++) {
		int start0 = hop_len * static_cast<int>(i);
		int end0 = start0 + s21;
		int start1 = end0;
		int end1 = start1 + s20;
		for (int j = 0; j < s20; j++) {
			Sx[j][i] = xp[start1 + j];
		}
		for (int j = 0; j < s20; j++) {
			Sx[s20 + j][i] = xp[start0 + j];
		}
	}

	for (int i = 0; i < 192; i++) {
		for (int j = 0; j < 257; j++) {
			Sx[j][i] *= window[j]; 
		}
		rfft(Sx[i], out[i]);
	}
}


static void istft(array<array<complex<double>, 257>, 64>& Sx, array<double, 257>& temp,  array<array<double, 257>, 64>& xbuf,  array<double, 384>& buffer, array<double, 8192>& output, const array<double, 512>& window) {
	const int n_fft = 512;
	const int win_len = 512;
	const int N = 8192;
	const int hop_len = 128;

	// Compute the real part of the inverse FFT and shift the values in xbuf using fftshift
	for (int i = 0; i < Sx.size(); i++) {
		irfft_fftshift_1d(Sx[i], xbuf[i]);
	}

	// Back to time domain
	int n = 0;
	for (int i = 0; i < xbuf[0].size(); i++) {
		array<double, 257> processing = { 0 };
		for (int j = 0; j < xbuf.size(); j++) {
			processing[j] = xbuf[j][i] * window[j];
		}
		array<double, 128> out = { 0 };
		for (int j = 0; j < hop_len; j++) {
			out[j] = processing[j] + buffer[j];
			buffer[j] = buffer[j + hop_len] + processing[j + hop_len];
		}
		for (int j = 0; j < out.size(); j++) {
			output[n + j] = out[j];
		}
		n += hop_len;
	}

	// Commit changes to persistent buffer
	for (int i = 0; i < hop_len; i++) {
		buffer[i] = buffer[i + hop_len];
	}
	 std::fill(std::end(buffer) - hop_len, std::end(buffer), 0.0); //fill end with zeros

}



vector<int> slicing(vector<int> arr,
	int X, int Y)
{
	// Starting and ending indices
	int start = X;
	int end = Y;

	// To store the sliced vector
	vector<int> result(end - start + 1);

	// Loop over the elements in the range [start, end] and copy them to the result vector
	for (int i = start; i <= end; i++) {
		result[i - start] = arr[i];
	}

	// Return the final sliced vector
	return result;
}



#define EXPORT    __declspec( dllexport )



class  Filter  //because we instance the Filter_ as a parent of this one, we instantiate it.
{
private:
	class Data {
	public:
		//all data is allocated here.  Doing so ensures it is allocated on the stack.
		//Design decisions and considerations:
		// This code needs to be portable to every other language, and easy to understand, without needing to understand language specific shenanigans.
		//stack is generally faster than heap fiddling, and for a situation where you're only doing in-place transformation, like I am, it's fine to just allocate a chunk of memory.
		//we never need more memory than we need to run the algorithm once. If we do, it's a sign of a memory leak. This algorithm may have to run on embedded systems, so the behavior must be provably correct and fast.
		// all writes should be in-place. No objects should ever be created or destroyed.
		//as a result, the use of vectors is banned in this class. So is the heap. no dynamically growing types. No allocations beyond a static range.
		//we only need two data types- int and double. We also need complex, which is two doubles. We don't need int because we never go that high.
		//at most, we need the following spaces of memory:
		//all data is allocated here. 
		//Design decisions and considerations:
		// This code needs to be portable to every other language, and easy to understand, without needing to understand language specific shenanigans.
		//stack is generally faster than heap fiddling, and for a situation where you're only doing in-place transformation, like I am, it's fine to just allocate a chunk of memory.
		//we never need more memory than we need to run the algorithm once. If we do, it's a sign of a memory leak. This algorithm may have to run on embedded systems, so the behavior must be provably correct and fast.
		// all writes should be in-place. No objects should ever be created or destroyed.
		//as a result, the use of vectors is banned in this class. So is the heap. no dynamically growing types. No allocations beyond a static range.
		//we only need two data types- int and double. We also need complex, which is two doubles. We don't need int because we never go that high.
		//at most, we need the following spaces of memory:
		static array<double, 24576> audio;
		static array<double, 384> buffer;
		static array<double, 192> entropy_unmasked;
		static array<double, 204> entropy_padded; //192 + 12

		static array<double, 8192> output;
		static array<double, 192> entropy;
		static array<double, 257> temp;
		static array<int, 192> entropy_thresholded;
		static array<double, 25087> audio_padded;


		static int flag, maxstreak, count;
		static double t, initial, multiplier;


		//note: shifted_ means ifftshift was applied beforehand. Do not use the included stft class without shifting windows.

		const array<double, 512> shifted_logistic_window{ 1., 0.94409333, 0.88818667, 0.85535081, 0.83195987, 0.81374363, 0.79880003, 0.78611443, 0.77508119, 0.76530966, 0.75653308, 0.7485612, 0.74125354, 0.73450341, 0.72822787, 0.72236118, 0.71685033, 0.71165198, 0.7067302, 0.70205491, 0.69760064, 0.69334567, 0.68927131, 0.68536134, 0.68160163, 0.67797977, 0.67448482, 0.67110707, 0.66783786, 0.66466946, 0.6615949, 0.65860793, 0.65570285, 0.65287452, 0.65011824, 0.64742972, 0.64480505, 0.64224061, 0.63973308, 0.63727941, 0.63487677, 0.63252254, 0.63021427, 0.62794972, 0.62572677, 0.62354345, 0.62139793, 0.61928848, 0.6172135, 0.61517145, 0.61316093, 0.61118059, 0.60922916, 0.60730545, 0.60540833, 0.60353673, 0.60168964, 0.59986611, 0.59806522, 0.5962861, 0.59452794, 0.59278996, 0.5910714, 0.58937155, 0.58768973, 0.5860253, 0.58437763, 0.58274613, 0.58113023, 0.57952939, 0.57794307, 0.57637077, 0.57481202, 0.57326635, 0.5717333, 0.57021245, 0.56870339, 0.56720571, 0.56571903, 0.56424297, 0.56277718, 0.56132129, 0.55987498, 0.55843793, 0.5570098, 0.55559029, 0.55417912, 0.55277598, 0.55138059, 0.54999269, 0.548612, 0.54723826, 0.54587123, 0.54451066, 0.5431563, 0.54180793, 0.54046531, 0.53912822, 0.53779645, 0.53646977, 0.53514798, 0.53383087, 0.53251824, 0.53120989, 0.52990563, 0.52860527, 0.52730862, 0.5260155, 0.52472572, 0.5234391, 0.52215548, 0.52087466, 0.51959649, 0.51832079, 0.51704739, 0.51577613, 0.51450685, 0.51323937, 0.51197354, 0.5107092, 0.50944618, 0.50818433, 0.5069235, 0.50566352, 0.50440424, 0.5031455, 0.50188714, 0.50062902, 0.49937098, 0.49811286, 0.4968545, 0.49559576, 0.49433648, 0.4930765, 0.49181567, 0.49055382, 0.4892908, 0.48802646, 0.48676063, 0.48549315, 0.48422387, 0.48295261, 0.48167921, 0.48040351, 0.47912534, 0.47784452, 0.4765609, 0.47527428, 0.4739845, 0.47269138, 0.47139473, 0.47009437, 0.46879011, 0.46748176, 0.46616913, 0.46485202, 0.46353023, 0.46220355, 0.46087178, 0.45953469, 0.45819207, 0.4568437, 0.45548934, 0.45412877, 0.45276174, 0.451388, 0.45000731, 0.44861941, 0.44722402, 0.44582088, 0.44440971, 0.4429902, 0.44156207, 0.44012502, 0.43867871, 0.43722282, 0.43575703, 0.43428097, 0.43279429, 0.43129661, 0.42978755, 0.4282667, 0.42673365, 0.42518798, 0.42362923, 0.42205693, 0.42047061, 0.41886977, 0.41725387, 0.41562237, 0.4139747, 0.41231027, 0.41062845, 0.4089286, 0.40721004, 0.40547206, 0.4037139, 0.40193478, 0.40013389, 0.39831036, 0.39646327, 0.39459167, 0.39269455, 0.39077084, 0.38881941, 0.38683907, 0.38482855, 0.3827865, 0.38071152, 0.37860207, 0.37645655, 0.37427323, 0.37205028, 0.36978573, 0.36747746, 0.36512323, 0.36272059, 0.36026692, 0.35775939, 0.35519495, 0.35257028, 0.34988176, 0.34712548, 0.34429715, 0.34139207, 0.3384051, 0.33533054, 0.33216214, 0.32889293, 0.32551518, 0.32202023, 0.31839837, 0.31463866, 0.31072869, 0.30665433, 0.30239936, 0.29794509, 0.2932698, 0.28834802, 0.28314967, 0.27763882, 0.27177213, 0.26549659, 0.25874646, 0.2514388, 0.24346692, 0.23469034, 0.22491881, 0.21388557, 0.20119997, 0.18625637, 0.16804013, 0.14464919, 0.11181333, 0.05590667, 0., 0., 0.05590667, 0.11181333, 0.14464919, 0.16804013, 0.18625637, 0.20119997, 0.21388557, 0.22491881, 0.23469034, 0.24346692, 0.2514388, 0.25874646, 0.26549659, 0.27177213, 0.27763882, 0.28314967, 0.28834802, 0.2932698, 0.29794509, 0.30239936, 0.30665433, 0.31072869, 0.31463866, 0.31839837, 0.32202023, 0.32551518, 0.32889293, 0.33216214, 0.33533054, 0.3384051, 0.34139207, 0.34429715, 0.34712548, 0.34988176, 0.35257028, 0.35519495, 0.35775939, 0.36026692, 0.36272059, 0.36512323, 0.36747746, 0.36978573, 0.37205028, 0.37427323, 0.37645655, 0.37860207, 0.38071152, 0.3827865, 0.38482855, 0.38683907, 0.38881941, 0.39077084, 0.39269455, 0.39459167, 0.39646327, 0.39831036, 0.40013389, 0.40193478, 0.4037139, 0.40547206, 0.40721004, 0.4089286, 0.41062845, 0.41231027, 0.4139747, 0.41562237, 0.41725387, 0.41886977, 0.42047061, 0.42205693, 0.42362923, 0.42518798, 0.42673365, 0.4282667, 0.42978755, 0.43129661, 0.43279429, 0.43428097, 0.43575703, 0.43722282, 0.43867871, 0.44012502, 0.44156207, 0.4429902, 0.44440971, 0.44582088, 0.44722402, 0.44861941, 0.45000731, 0.451388, 0.45276174, 0.45412877, 0.45548934, 0.4568437, 0.45819207, 0.45953469, 0.46087178, 0.46220355, 0.46353023, 0.46485202, 0.46616913, 0.46748176, 0.46879011, 0.47009437, 0.47139473, 0.47269138, 0.4739845, 0.47527428, 0.4765609, 0.47784452, 0.47912534, 0.48040351, 0.48167921, 0.48295261, 0.48422387, 0.48549315, 0.48676063, 0.48802646, 0.4892908, 0.49055382, 0.49181567, 0.4930765, 0.49433648, 0.49559576, 0.4968545, 0.49811286, 0.49937098, 0.50062902, 0.50188714, 0.5031455, 0.50440424, 0.50566352, 0.5069235, 0.50818433, 0.50944618, 0.5107092, 0.51197354, 0.51323937, 0.51450685, 0.51577613, 0.51704739, 0.51832079, 0.51959649, 0.52087466, 0.52215548, 0.5234391, 0.52472572, 0.5260155, 0.52730862, 0.52860527, 0.52990563, 0.53120989, 0.53251824, 0.53383087, 0.53514798, 0.53646977, 0.53779645, 0.53912822, 0.54046531, 0.54180793, 0.5431563, 0.54451066, 0.54587123, 0.54723826, 0.548612, 0.54999269, 0.55138059, 0.55277598, 0.55417912, 0.55559029, 0.5570098, 0.55843793, 0.55987498, 0.56132129, 0.56277718, 0.56424297, 0.56571903, 0.56720571, 0.56870339, 0.57021245, 0.5717333, 0.57326635, 0.57481202, 0.57637077, 0.57794307, 0.57952939, 0.58113023, 0.58274613, 0.58437763, 0.5860253, 0.58768973, 0.58937155, 0.5910714, 0.59278996, 0.59452794, 0.5962861, 0.59806522, 0.59986611, 0.60168964, 0.60353673, 0.60540833, 0.60730545, 0.60922916, 0.61118059, 0.61316093, 0.61517145, 0.6172135, 0.61928848, 0.62139793, 0.62354345, 0.62572677, 0.62794972, 0.63021427, 0.63252254, 0.63487677, 0.63727941, 0.63973308, 0.64224061, 0.64480505, 0.64742972, 0.65011824, 0.65287452, 0.65570285, 0.65860793, 0.6615949, 0.66466946, 0.66783786, 0.67110707, 0.67448482, 0.67797977, 0.68160163, 0.68536134, 0.68927131, 0.69334567, 0.69760064, 0.70205491, 0.7067302, 0.71165198, 0.71685033, 0.72236118, 0.72822787, 0.73450341, 0.74125354, 0.7485612, 0.75653308, 0.76530966, 0.77508119, 0.78611443, 0.79880003, 0.81374363, 0.83195987, 0.85535081, 0.88818667, 0.94409333, 1. };
		const array<double, 512> shifted_hann_window = { 9.99990551e-01, 9.99914959e-01, 9.99763787e-01, 9.99537058e-01, 9.99234805e-01, 9.98857075e-01, 9.98403924e-01, 9.97875422e-01, 9.97271648e-01, 9.96592693e-01, 9.95838660e-01, 9.95009663e-01, 9.94105827e-01, 9.93127290e-01, 9.92074198e-01, 9.90946711e-01, 9.89745000e-01, 9.88469246e-01, 9.87119643e-01, 9.85696393e-01, 9.84199713e-01, 9.82629829e-01, 9.80986977e-01, 9.79271407e-01, 9.77483377e-01, 9.75623159e-01, 9.73691033e-01, 9.71687291e-01, 9.69612237e-01, 9.67466184e-01, 9.65249456e-01, 9.62962389e-01, 9.60605328e-01, 9.58178630e-01, 9.55682662e-01, 9.53117800e-01, 9.50484434e-01, 9.47782960e-01, 9.45013788e-01, 9.42177336e-01, 9.39274033e-01, 9.36304317e-01, 9.33268638e-01, 9.30167455e-01, 9.27001237e-01, 9.23770461e-01, 9.20475618e-01, 9.17117204e-01, 9.13695728e-01, 9.10211707e-01, 9.06665667e-01, 9.03058145e-01, 8.99389686e-01, 8.95660845e-01, 8.91872186e-01, 8.88024281e-01, 8.84117711e-01, 8.80153069e-01, 8.76130952e-01, 8.72051970e-01, 8.67916738e-01, 8.63725883e-01, 8.59480037e-01, 8.55179843e-01, 8.50825950e-01, 8.46419017e-01, 8.41959711e-01, 8.37448705e-01, 8.32886681e-01, 8.28274329e-01, 8.23612347e-01, 8.18901439e-01, 8.14142317e-01, 8.09335702e-01, 8.04482319e-01, 7.99582902e-01, 7.94638193e-01, 7.89648938e-01, 7.84615893e-01, 7.79539817e-01, 7.74421479e-01, 7.69261652e-01, 7.64061117e-01, 7.58820659e-01, 7.53541070e-01, 7.48223150e-01, 7.42867702e-01, 7.37475536e-01, 7.32047467e-01, 7.26584315e-01, 7.21086907e-01, 7.15556073e-01, 7.09992651e-01, 7.04397480e-01, 6.98771407e-01, 6.93115283e-01, 6.87429962e-01, 6.81716305e-01, 6.75975174e-01, 6.70207439e-01, 6.64413970e-01, 6.58595644e-01, 6.52753341e-01, 6.46887944e-01, 6.41000339e-01, 6.35091417e-01, 6.29162070e-01, 6.23213197e-01, 6.17245695e-01, 6.11260467e-01, 6.05258418e-01, 5.99240456e-01, 5.93207489e-01, 5.87160431e-01, 5.81100196e-01, 5.75027699e-01, 5.68943859e-01, 5.62849596e-01, 5.56745831e-01, 5.50633486e-01, 5.44513487e-01, 5.38386758e-01, 5.32254225e-01, 5.26116815e-01, 5.19975458e-01, 5.13831080e-01, 5.07684611e-01, 5.01536980e-01, 4.95389117e-01, 4.89241951e-01, 4.83096412e-01, 4.76953428e-01, 4.70813928e-01, 4.64678841e-01, 4.58549094e-01, 4.52425614e-01, 4.46309327e-01, 4.40201156e-01, 4.34102027e-01, 4.28012860e-01, 4.21934577e-01, 4.15868096e-01, 4.09814335e-01, 4.03774209e-01, 3.97748631e-01, 3.91738511e-01, 3.85744760e-01, 3.79768282e-01, 3.73809982e-01, 3.67870760e-01, 3.61951513e-01, 3.56053138e-01, 3.50176526e-01, 3.44322565e-01, 3.38492141e-01, 3.32686134e-01, 3.26905422e-01, 3.21150881e-01, 3.15423378e-01, 3.09723782e-01, 3.04052952e-01, 2.98411747e-01, 2.92801019e-01, 2.87221617e-01, 2.81674384e-01, 2.76160159e-01, 2.70679775e-01, 2.65234062e-01, 2.59823842e-01, 2.54449933e-01, 2.49113148e-01, 2.43814294e-01, 2.38554171e-01, 2.33333576e-01, 2.28153297e-01, 2.23014117e-01, 2.17916814e-01, 2.12862158e-01, 2.07850913e-01, 2.02883837e-01, 1.97961681e-01, 1.93085190e-01, 1.88255099e-01, 1.83472140e-01, 1.78737036e-01, 1.74050502e-01, 1.69413247e-01, 1.64825973e-01, 1.60289372e-01, 1.55804131e-01, 1.51370928e-01, 1.46990432e-01, 1.42663307e-01, 1.38390206e-01, 1.34171776e-01, 1.30008654e-01, 1.25901469e-01, 1.21850843e-01, 1.17857388e-01, 1.13921708e-01, 1.10044397e-01, 1.06226043e-01, 1.02467221e-01, 9.87685015e-02, 9.51304424e-02, 9.15535940e-02, 8.80384971e-02, 8.45856832e-02, 8.11956742e-02, 7.78689827e-02, 7.46061116e-02, 7.14075543e-02, 6.82737943e-02, 6.52053053e-02, 6.22025514e-02, 5.92659864e-02, 5.63960544e-02, 5.35931893e-02, 5.08578147e-02, 4.81903443e-02, 4.55911813e-02, 4.30607187e-02, 4.05993391e-02, 3.82074146e-02, 3.58853068e-02, 3.36333667e-02, 3.14519350e-02, 2.93413412e-02, 2.73019047e-02, 2.53339336e-02, 2.34377255e-02, 2.16135671e-02, 1.98617342e-02, 1.81824916e-02, 1.65760932e-02, 1.50427819e-02, 1.35827895e-02, 1.21963367e-02, 1.08836332e-02, 9.64487731e-03, 8.48025644e-03, 7.38994662e-03, 6.37411270e-03, 5.43290826e-03, 4.56647559e-03, 3.77494569e-03, 3.05843822e-03, 2.41706151e-03, 1.85091253e-03, 1.36007687e-03, 9.44628746e-04, 6.04630957e-04, 3.40134910e-04, 1.51180595e-04, 3.77965773e-05, 0.00000000e+00, 0.00000000e+00, 3.77965773e-05, 1.51180595e-04, 3.40134910e-04, 6.04630957e-04, 9.44628746e-04, 1.36007687e-03, 1.85091253e-03, 2.41706151e-03, 3.05843822e-03, 3.77494569e-03, 4.56647559e-03, 5.43290826e-03, 6.37411270e-03, 7.38994662e-03, 8.48025644e-03, 9.64487731e-03, 1.08836332e-02, 1.21963367e-02, 1.35827895e-02, 1.50427819e-02, 1.65760932e-02, 1.81824916e-02, 1.98617342e-02, 2.16135671e-02, 2.34377255e-02, 2.53339336e-02, 2.73019047e-02, 2.93413412e-02, 3.14519350e-02, 3.36333667e-02, 3.58853068e-02, 3.82074146e-02, 4.05993391e-02, 4.30607187e-02, 4.55911813e-02, 4.81903443e-02, 5.08578147e-02, 5.35931893e-02, 5.63960544e-02, 5.92659864e-02, 6.22025514e-02, 6.52053053e-02, 6.82737943e-02, 7.14075543e-02, 7.46061116e-02, 7.78689827e-02, 8.11956742e-02, 8.45856832e-02, 8.80384971e-02, 9.15535940e-02, 9.51304424e-02, 9.87685015e-02, 1.02467221e-01, 1.06226043e-01, 1.10044397e-01, 1.13921708e-01, 1.17857388e-01, 1.21850843e-01, 1.25901469e-01, 1.30008654e-01, 1.34171776e-01, 1.38390206e-01, 1.42663307e-01, 1.46990432e-01, 1.51370928e-01, 1.55804131e-01, 1.60289372e-01, 1.64825973e-01, 1.69413247e-01, 1.74050502e-01, 1.78737036e-01, 1.83472140e-01, 1.88255099e-01, 1.93085190e-01, 1.97961681e-01, 2.02883837e-01, 2.07850913e-01, 2.12862158e-01, 2.17916814e-01, 2.23014117e-01, 2.28153297e-01, 2.33333576e-01, 2.38554171e-01, 2.43814294e-01, 2.49113148e-01, 2.54449933e-01, 2.59823842e-01, 2.65234062e-01, 2.70679775e-01, 2.76160159e-01, 2.81674384e-01, 2.87221617e-01, 2.92801019e-01, 2.98411747e-01, 3.04052952e-01, 3.09723782e-01, 3.15423378e-01, 3.21150881e-01, 3.26905422e-01, 3.32686134e-01, 3.38492141e-01, 3.44322565e-01, 3.50176526e-01, 3.56053138e-01, 3.61951513e-01, 3.67870760e-01, 3.73809982e-01, 3.79768282e-01, 3.85744760e-01, 3.91738511e-01, 3.97748631e-01, 4.03774209e-01, 4.09814335e-01, 4.15868096e-01, 4.21934577e-01, 4.28012860e-01, 4.34102027e-01, 4.40201156e-01, 4.46309327e-01, 4.52425614e-01, 4.58549094e-01, 4.64678841e-01, 4.70813928e-01, 4.76953428e-01, 4.83096412e-01, 4.89241951e-01, 4.95389117e-01, 5.01536980e-01, 5.07684611e-01, 5.13831080e-01, 5.19975458e-01, 5.26116815e-01, 5.32254225e-01, 5.38386758e-01, 5.44513487e-01, 5.50633486e-01, 5.56745831e-01, 5.62849596e-01, 5.68943859e-01, 5.75027699e-01, 5.81100196e-01, 5.87160431e-01, 5.93207489e-01, 5.99240456e-01, 6.05258418e-01, 6.11260467e-01, 6.17245695e-01, 6.23213197e-01, 6.29162070e-01, 6.35091417e-01, 6.41000339e-01, 6.46887944e-01, 6.52753341e-01, 6.58595644e-01, 6.64413970e-01, 6.70207439e-01, 6.75975174e-01, 6.81716305e-01, 6.87429962e-01, 6.93115283e-01, 6.98771407e-01, 7.04397480e-01, 7.09992651e-01, 7.15556073e-01, 7.21086907e-01, 7.26584315e-01, 7.32047467e-01, 7.37475536e-01, 7.42867702e-01, 7.48223150e-01, 7.53541070e-01, 7.58820659e-01, 7.64061117e-01, 7.69261652e-01, 7.74421479e-01, 7.79539817e-01, 7.84615893e-01, 7.89648938e-01, 7.94638193e-01, 7.99582902e-01, 8.04482319e-01, 8.09335702e-01, 8.14142317e-01, 8.18901439e-01, 8.23612347e-01, 8.28274329e-01, 8.32886681e-01, 8.37448705e-01, 8.41959711e-01, 8.46419017e-01, 8.50825950e-01, 8.55179843e-01, 8.59480037e-01, 8.63725883e-01, 8.67916738e-01, 8.72051970e-01, 8.76130952e-01, 8.80153069e-01, 8.84117711e-01, 8.88024281e-01, 8.91872186e-01, 8.95660845e-01, 8.99389686e-01, 9.03058145e-01, 9.06665667e-01, 9.10211707e-01, 9.13695728e-01, 9.17117204e-01, 9.20475618e-01, 9.23770461e-01, 9.27001237e-01, 9.30167455e-01, 9.33268638e-01, 9.36304317e-01, 9.39274033e-01, 9.42177336e-01, 9.45013788e-01, 9.47782960e-01, 9.50484434e-01, 9.53117800e-01, 9.55682662e-01, 9.58178630e-01, 9.60605328e-01, 9.62962389e-01, 9.65249456e-01, 9.67466184e-01, 9.69612237e-01, 9.71687291e-01, 9.73691033e-01, 9.75623159e-01, 9.77483377e-01, 9.79271407e-01, 9.80986977e-01, 9.82629829e-01, 9.84199713e-01, 9.85696393e-01, 9.87119643e-01, 9.88469246e-01, 9.89745000e-01, 9.90946711e-01, 9.92074198e-01, 9.93127290e-01, 9.94105827e-01, 9.95009663e-01, 9.95838660e-01, 9.96592693e-01, 9.97271648e-01, 9.97875422e-01, 9.98403924e-01, 9.98857075e-01, 9.99234805e-01, 9.99537058e-01, 9.99763787e-01, 9.99914959e-01, 9.99990551e-01 };
		const array<double, 512> logit_distribution = { 0.0, 0.08441118, 0.16882236, 0.2197072, 0.25693163, 0.28672628, 0.31187091, 0.33385255, 0.35356306, 0.37158192, 0.38830914, 0.40403462, 0.41897721, 0.43330834, 0.44716693, 0.46066936, 0.47391649, 0.4869987, 0.5, 0.5130013, 0.52608351, 0.53933064, 0.55283307, 0.56669166, 0.58102279, 0.59596538, 0.61169086, 0.62841808, 0.64643694, 0.66614745, 0.68812909, 0.71327372, 0.74306837, 0.7802928, 0.83117764, 0.91558882, 1.0 };
		const array<double, 512> synthesis_window = { 0.00000000e+00,2.52493737e-05,1.00993617e-04,2.27221124e-04,4.03912573e-04,6.31040943e-04,9.08571512e-04,1.23646188e-03,1.61466197e-03,2.04311406e-03,2.52175277e-03,3.05050510e-03,3.62929044e-03,4.25802059e-03,4.93659976e-03,5.66492464e-03,6.44288435e-03,7.27036053e-03,8.14722732e-03,9.07335139e-03,1.00485920e-02,1.10728009e-02,1.21458227e-02,1.32674943e-02,1.44376456e-02,1.56560990e-02,1.69226697e-02,1.82371657e-02,1.95993878e-02,2.10091294e-02,2.24661772e-02,2.39703105e-02,2.55213015e-02,2.71189155e-02,2.87629109e-02,3.04530390e-02,3.21890442e-02,3.39706641e-02,3.57976294e-02,3.76696640e-02,3.95864853e-02,4.15478036e-02,4.35533228e-02,4.56027403e-02,4.76957466e-02,4.98320260e-02,5.20112561e-02,5.42331081e-02,5.64972471e-02,5.88033315e-02,6.11510136e-02,6.35399396e-02,6.59697493e-02,6.84400765e-02,7.09505488e-02,7.35007880e-02,7.60904099e-02,7.87190242e-02,8.13862349e-02,8.40916401e-02,8.68348324e-02,8.96153985e-02,9.24329196e-02,9.52869711e-02,9.81771232e-02,1.01102941e-01,1.04063982e-01,1.07059803e-01,1.10089950e-01,1.13153968e-01,1.16251394e-01,1.19381763e-01,1.22544602e-01,1.25739435e-01,1.28965780e-01,1.32223151e-01,1.35511057e-01,1.38829001e-01,1.42176485e-01,1.45553002e-01,1.48958043e-01,1.52391095e-01,1.55851640e-01,1.59339154e-01,1.62853113e-01,1.66392985e-01,1.69958235e-01,1.73548325e-01,1.77162713e-01,1.80800852e-01,1.84462192e-01,1.88146180e-01,1.91852259e-01,1.95579867e-01,1.99328441e-01,2.03097413e-01,2.06886212e-01,2.10694266e-01,2.14520996e-01,2.18365823e-01,2.22228164e-01,2.26107433e-01,2.30003042e-01,2.33914400e-01,2.37840913e-01,2.41781985e-01,2.45737016e-01,2.49705407e-01,2.53686555e-01,2.57679853e-01,2.61684694e-01,2.65700470e-01,2.69726568e-01,2.73762377e-01,2.77807281e-01,2.81860664e-01,2.85921908e-01,2.89990394e-01,2.94065503e-01,2.98146611e-01,3.02233096e-01,3.06324335e-01,3.10419702e-01,3.14518572e-01,3.18620319e-01,3.22724316e-01,3.26829934e-01,3.30936547e-01,3.35043526e-01,3.39150246e-01,3.43256086e-01,3.47360427e-01,3.51462648e-01,3.55562129e-01,3.59658251e-01,3.63750398e-01,3.67837950e-01,3.71920292e-01,3.75996808e-01,3.80066883e-01,3.84129905e-01,3.88185260e-01,3.92232339e-01,3.96270531e-01,4.00299230e-01,4.04317828e-01,4.08325721e-01,4.12322306e-01,4.16306982e-01,4.20279150e-01,4.24238213e-01,4.28183575e-01,4.32114645e-01,4.36030831e-01,4.39931546e-01,4.43816203e-01,4.47684220e-01,4.51535015e-01,4.55368011e-01,4.59182632e-01,4.62978306e-01,4.66754464e-01,4.70510539e-01,4.74245967e-01,4.77960189e-01,4.81652647e-01,4.85322788e-01,4.88970060e-01,4.92593918e-01,4.96193817e-01,4.99769218e-01,5.03319584e-01,5.06844384e-01,5.10343088e-01,5.13815172e-01,5.17260116e-01,5.20677401e-01,5.24066516e-01,5.27426952e-01,5.30758205e-01,5.34059774e-01,5.37331165e-01,5.40571886e-01,5.43781450e-01,5.46959376e-01,5.50105185e-01,5.53218405e-01,5.56298569e-01,5.59345212e-01,5.62357878e-01,5.65336111e-01,5.68279465e-01,5.71187495e-01,5.74059764e-01,5.76895840e-01,5.79695293e-01,5.82457703e-01,5.85182652e-01,5.87869728e-01,5.90518527e-01,5.93128647e-01,5.95699693e-01,5.98231278e-01,6.00723016e-01,6.03174532e-01,6.05585452e-01,6.07955411e-01,6.10284050e-01,6.12571014e-01,6.14815956e-01,6.17018534e-01,6.19178413e-01,6.21295262e-01,6.23368760e-01,6.25398589e-01,6.27384439e-01,6.29326006e-01,6.31222993e-01,6.33075109e-01,6.34882068e-01,6.36643595e-01,6.38359416e-01,6.40029269e-01,6.41652895e-01,6.43230043e-01,6.44760469e-01,6.46243937e-01,6.47680215e-01,6.49069080e-01,6.50410317e-01,6.51703715e-01,6.52949072e-01,6.54146194e-01,6.55294893e-01,6.56394987e-01,6.57446303e-01,6.58448674e-01,6.59401943e-01,6.60305956e-01,6.61160570e-01,6.61965647e-01,6.62721059e-01,6.63426683e-01,6.64082404e-01,6.64688115e-01,6.65243717e-01,6.65749117e-01,6.66204231e-01,6.66608983e-01,6.66963302e-01,6.67267128e-01,6.67520406e-01,6.67723090e-01,6.67875141e-01,6.67976529e-01,6.68027230e-01,6.68027230e-01,6.67976529e-01,6.67875141e-01,6.67723090e-01,6.67520406e-01,6.67267128e-01,6.66963302e-01,6.66608983e-01,6.66204231e-01,6.65749117e-01,6.65243717e-01,6.64688115e-01,6.64082404e-01,6.63426683e-01,6.62721059e-01,6.61965647e-01,6.61160570e-01,6.60305956e-01,6.59401943e-01,6.58448674e-01,6.57446303e-01,6.56394987e-01,6.55294893e-01,6.54146194e-01,6.52949072e-01,6.51703715e-01,6.50410317e-01,6.49069080e-01,6.47680215e-01,6.46243937e-01,6.44760469e-01,6.43230043e-01,6.41652895e-01,6.40029269e-01,6.38359416e-01,6.36643595e-01,6.34882068e-01,6.33075109e-01,6.31222993e-01,6.29326006e-01,6.27384439e-01,6.25398589e-01,6.23368760e-01,6.21295262e-01,6.19178413e-01,6.17018534e-01,6.14815956e-01,6.12571014e-01,6.10284050e-01,6.07955411e-01,6.05585452e-01,6.03174532e-01,6.00723016e-01,5.98231278e-01,5.95699693e-01,5.93128647e-01,5.90518527e-01,5.87869728e-01,5.85182652e-01,5.82457703e-01,5.79695293e-01,5.76895840e-01,5.74059764e-01,5.71187495e-01,5.68279465e-01,5.65336111e-01,5.62357878e-01,5.59345212e-01,5.56298569e-01,5.53218405e-01,5.50105185e-01,5.46959376e-01,5.43781450e-01,5.40571886e-01,5.37331165e-01,5.34059774e-01,5.30758205e-01,5.27426952e-01,5.24066516e-01,5.20677401e-01,5.17260116e-01,5.13815172e-01,5.10343088e-01,5.06844384e-01,5.03319584e-01,4.99769218e-01,4.96193817e-01,4.92593918e-01,4.88970060e-01,4.85322788e-01,4.81652647e-01,4.77960189e-01,4.74245967e-01,4.70510539e-01,4.66754464e-01,4.62978306e-01,4.59182632e-01,4.55368011e-01,4.51535015e-01,4.47684220e-01,4.43816203e-01,4.39931546e-01,4.36030831e-01,4.32114645e-01,4.28183575e-01,4.24238213e-01,4.20279150e-01,4.16306982e-01,4.12322306e-01,4.08325721e-01,4.04317828e-01,4.00299230e-01,3.96270531e-01,3.92232339e-01,3.88185260e-01,3.84129905e-01,3.80066883e-01,3.75996808e-01,3.71920292e-01,3.67837950e-01,3.63750398e-01,3.59658251e-01,3.55562129e-01,3.51462648e-01,3.47360427e-01,3.43256086e-01,3.39150246e-01,3.35043526e-01,3.30936547e-01,3.26829934e-01,3.22724316e-01,3.18620319e-01,3.14518572e-01,3.10419702e-01,3.06324335e-01,3.02233096e-01,2.98146611e-01,2.94065503e-01,2.89990394e-01,2.85921908e-01,2.81860664e-01,2.77807281e-01,2.73762377e-01,2.69726568e-01,2.65700470e-01,2.61684694e-01,2.57679853e-01,2.53686555e-01,2.49705407e-01,2.45737016e-01,2.41781985e-01,2.37840913e-01,2.33914400e-01,2.30003042e-01,2.26107433e-01,2.22228164e-01,2.18365823e-01,2.14520996e-01,2.10694266e-01,2.06886212e-01,2.03097413e-01,1.99328441e-01,1.95579867e-01,1.91852259e-01,1.88146180e-01,1.84462192e-01,1.80800852e-01,1.77162713e-01,1.73548325e-01,1.69958235e-01,1.66392985e-01,1.62853113e-01,1.59339154e-01,1.55851640e-01,1.52391095e-01,1.48958043e-01,1.45553002e-01,1.42176485e-01,1.38829001e-01,1.35511057e-01,1.32223151e-01,1.28965780e-01,1.25739435e-01,1.22544602e-01,1.19381763e-01,1.16251394e-01,1.13153968e-01,1.10089950e-01,1.07059803e-01,1.04063982e-01,1.01102941e-01,9.81771232e-02,9.52869711e-02,9.24329196e-02,8.96153985e-02,8.68348324e-02,8.40916401e-02,8.13862349e-02,7.87190242e-02,7.60904099e-02,7.35007880e-02,7.09505488e-02,6.84400765e-02,6.59697493e-02,6.35399396e-02,6.11510136e-02,5.88033315e-02,5.64972471e-02,5.42331081e-02,5.20112561e-02,4.98320260e-02,4.76957466e-02,4.56027403e-02,4.35533228e-02,4.15478036e-02,3.95864853e-02,3.76696640e-02,3.57976294e-02,3.39706641e-02,3.21890442e-02,3.04530390e-02,2.87629109e-02,2.71189155e-02,2.55213015e-02,2.39703105e-02,2.24661772e-02,2.10091294e-02,1.95993878e-02,1.82371657e-02,1.69226697e-02,1.56560990e-02,1.44376456e-02,1.32674943e-02,1.21458227e-02,1.10728009e-02,1.00485920e-02,9.07335139e-03,8.14722732e-03,7.27036053e-03,6.44288435e-03,5.66492464e-03,4.93659976e-03,4.25802059e-03,3.62929044e-03,3.05050510e-03,2.52175277e-03,2.04311406e-03,1.61466197e-03,1.23646188e-03,9.08571512e-04,6.31040943e-04,4.03912573e-04,2.27221124e-04,1.00993617e-04,2.52493737e-05,0.00000000e+00 };
		const array<double, 3> entropyweights = { 1.,1.,1. };
		const double empty[8192] = { 0 }; //just an empty set of zeros
		const int NBINS = 37;
		const double constant = 0.057;
		const double MAXIMUM = 0.6122169028112414;


		static array<array<complex<double>, 257>, 192> stft_complex;
		static array<array<double, 37>, 192> stft_real;

		static array<array<complex<double>, 257>, 64> stft_output;
		const array<array<complex<double>, 257>, 64> stft_zeros = { 0 }; // for use in the scenario two where we're processing the residual buffer but without the audio

		static array<array<double, 37>, 192> stft_real;
		static array<array<double, 37>, 192> filtered;
		static array<array<double, 257>, 64> real_buffer_istft;
		static array<array<double, 257>, 192> real_buffer_stft;

		//TODO: find a way to merge the uses of the above so that a mimimum in working memory can be utilized
	};

	Data a;
	

public:
	array<int, 8192> process_real(const array<int, 8192> input) {


		//in this function we are only to allocate and do reference based manipulation of data- never copy or move. 
		rotate(std::begin(a.audio), std::begin(a.audio) +8192, std::end(a.audio)); // Shift the values in the array 8192 values to the left.
		copy(std::begin(input), std::end(input), std::end(a.audio) - 8192); // Copy the contents of input into the last 8192 elements of audio.
		copy(std::begin(a.audio), std::end(a.audio), std::end(a.audio_padded) - 25342); // Copy(hopefully) audio into the correct offset of audio_padded.
		copy_backward(a.audio.begin() + 257, a.audio.begin() + 257 + 256, a.audio_padded.begin()); //perform the reflected padding
		copy_backward(a.audio.end() - 255, a.audio.end(), a.audio_padded.end()); //perform the reflected padding
		stft(a.audio_padded, a.shifted_logistic_window, a.real_buffer_stft, a.stft_complex);
		// Copy the first 37 rows of stft_complex to stft_real
		for (int i = 0; i < 37; i++) {
			for (int j = 0; j < 192; j++) {
				a.stft_real[j][i] = abs(a.stft_complex[j][i]);
			}
		}
		fast_entropy(a.stft_real, a.entropy_unmasked, a.logit_distribution);
		for (int i = 0; i < 192; i++) {
			if (isnan(a.entropy_unmasked[i])) {
				a.entropy_unmasked[i] = 0;
			}
		} 
		copy(std::begin(a.entropy_unmasked), std::end(a.entropy_unmasked), std::end(a.entropy_padded) - 198) //copy into middle of padding
		vector<double> sums = same_convolution_double_1d(x, weights);

		for (int i = 0; i < 192; i++) {
			a.entropy_unmasked[i+6] = a.entropy_unmasked[i+6]/3
		}

		double weight_double = static_cast<double>(w);
		for (double sum : sums) {
			sum /= weight_double;
		}

		return sums;
		moving_average(a.entropy_padded, n);



		if (*max_element(entropy.begin(), entropy.end()) > constant) {
			for (int i = 0; i < 192; i++) {
				if (entropy[i] < constant) {
					entropy[i] = 0.0;
				}
				else {
					entropy[i] = 1.0;
				}
			}

			vector<int> entropy2(entropy.begin(), entropy.end());

			vector<int>sliced = slicing(entropy2, 32, 160);
			int count = 0;
			for (auto val : sliced) {
				count += val;
			}

			maxstreak = longestConsecutive(slicing(entropy2, 32, 160));
			if (count > 22 || maxstreak > 16) {
				remove_outliers(a.entropy_thresholded, 0, 6, 1);
				remove_outliers(a.entropy_thresholded, 1, 2, 0);
				vector<double> logit_flat;
				logit_flat.reserve(logit_real.size() * logit_real[0].size());

				for (int i = 0; i < logit_real.size(); i++) {
					for (int j = 0; j < logit_real[0].size(); j++) {
						logit_flat[i * j] = logit_real[i][j];

					}
				}

				t = threshold(logit_flat);
				auto max_value = max_element(logit_flat.begin(), logit_flat.end());
				double initial = *max_value;

				vector<vector<double>> logit_real_2(logit_real.size(), vector<double>(logit_real[0].size(), 0.0));
				vector<vector<int>> previous(logit_real.size(), vector<int>(logit_real[0].size()));
				vector<vector<int>> next(logit_real.size(), vector<int>(logit_real[0].size()));
				vector<vector<double>> mask(stft_logit.size(), vector<double>(stft_logit[0].size()));

				logit_real_2 = sawtooth_filter(logit_real);

				previous = fast_peaks(logit_real_2, entropy2, t, entropy_unmasked, MAXIMUM);
				for (int i = 0; i < previous.size(); i++) {
					for (int j = 0; j < previous[0].size(); j++) {
						if (previous[i][j] == 0) {
							logit_real[i][j] = 0.0;

						}
					}
				}

				for (int i = 0; i < logit_real.size(); i++) {
					for (int j = 0; j < logit_real[0].size(); j++) {
						logit_flat[i * j] = logit_real[i][j];

					}
				}

				auto max_value2 = max_element(logit_flat.begin(), logit_flat.end());
				double multiplier = *max_value2;

				multiplier = multiplier / initial;
				multiplier = min(multiplier, 1.0);
				logit_real_2 = sawtooth_filter(logit_real);

				next = fast_peaks(logit_real_2, entropy2, t, entropy_unmasked, MAXIMUM);

				for (int i = 0; i < next.size(); i++) {
					for (int j = 0; j < next[0].size(); j++) {
						if (previous[i][j] > next[i][j] * multiplier) {
							mask[i][j] = previous[i][j];
						}
						else {
							mask[i][j] = next[i][j] * multiplier;
						}

					}
				}

				mask = sawtooth_filter(mask);
				mask = convolve_custom_filter_2d(mask, 13, 3, 3);
				// Compute hann

				stft_hann = stft(audio_vec, hann_window_vec);
				for (int i = 0; i < zeros[0].size(); i++) {
					for (int j = 0; j < NBINS; j++) {
						//apply the mask
						zeros[i][j] = zeros[i][j] * mask[i][j];
					}
				}

				flag = 1; //update the flag because we are good to go
				vector<double> output_vec = istft(zeros, buffer, synthesis_vec);

				return output_vec;
			}
		}
		if (flag == 1) {
			// Compute product using the residual buffer since on the last round it contained data
			vector<double> output_vec = istft(zeros, buffer, synthesis_vec);

			flag = 2; //update the flag since we processed zeros
			return output_vec;
		}
		//in the final case, we no longer need to process a residual buffer,
		//since the buffer was zeros on the last run, so no istft call is needed here.
		// Return zero array
		vector<double> nothing(arr, arr + sizeof(arr) / sizeof(arr[0]));

		return nothing;

	};
};






int main()
{
    std::cout << "Hello World!\n";
	Filter filter;
	vector<double> zeros = { 0 };
	vector<double> output = {0};
	output = filter.process_real(zeros);

}
