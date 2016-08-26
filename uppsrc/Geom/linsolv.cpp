#include "Geom.h"

namespace Upp {

LinearSolver::LinearSolver(int count, double tolerance)
: rows(count)
, col1(count + 1)
, tolerance(tolerance)
{
	ASSERT(tolerance >= 0);
	matrix.SetCount(count * (count + 1), 0);
	left_rows.SetCount(rows);
	left_cols.SetCount(rows);
	for(int i = 0; i < rows; i++)
		left_rows[i] = left_cols[i] = i;
}

void LinearSolver::AddLSI(const double *bases, double value)
{
	double *row = matrix.Begin();
	const double *bi = bases;
	for(int i = 0; i < rows; i++) {
		const double *bj = bases;
		for(int j = 0; j < rows; j++)
			*row++ += *bi * *bj++;
		*row++ += *bi++ * value;
	}
}

Vector<double> LinearSolver::Solve()
{
	while(!left_rows.IsEmpty())
	{
		int er = -1, ec = -1;
		double best = tolerance;
		for(int pr = 0; pr < left_rows.GetCount(); pr++)
		{
			const double *p = Row(left_rows[pr]);
			for(int pc = 0; pc < left_cols.GetCount(); pc++)
			{
				double v = fabs(p[left_cols[pc]]);
				if(v > best)
				{
					best = v;
					ec = pc;
					er = pr;
				}
			}
		}

		if(er < 0 || best <= 0) // just to be sure
		{ // no more fixed variables
			for(int i = 0; i < left_rows.GetCount(); i++)
				if(fabs(Right(left_rows[i])) > tolerance)
					return Vector<double>(); // error
			break;
		}

		int cr = left_rows[er], cc = left_cols[ec];

		int p = left_rows.Pop();
		if(er < left_rows.GetCount()) left_rows[er] = p;
		p = left_cols.Pop();
		if(ec < left_cols.GetCount()) left_cols[ec] = p;

		const double *src = Row(cr);
		const int *xb = left_cols.Begin(), *xe = left_cols.End();
		for(int i = 0; i < left_rows.GetCount(); i++)
		{
			double *dest = Row(left_rows[i]);
			double r = -dest[cc] / src[cc];
			for(const int *xp = xb; xp < xe; xp++)
				dest[*xp] += src[*xp] * r;
			dest[rows] += src[rows] * r;
		}

		pivots.Add(Point(cc, cr));
	}

	Vector<double> result;
	result.SetCount(rows, Null);
	for(int r = pivots.GetCount(); --r >= 0;)
	{
		Point pivot = pivots[r];
		const double *row = Row(pivot.y);
//		int px = pivot.x;
		double out = row[rows]; // right side
		for(int s = r; ++s < pivots.GetCount();)
		{
			int c = pivots[s].x;
			out -= row[c] * result[c];
		}
		result[pivot.x] = out / row[pivot.x];
	}

	return result;
}

void LinearSolver::SelfTest()
{
	for(int i = 0; i < 1000; i++)
	{
		int ord = rand() % 10 + 1;
		Vector<double> res;
		while(res.GetCount() < ord)
			res.Add(rand());
		LinearSolver ls(ord);
		for(int r = 0; r < ord; r++)
		{
			double rs = 0;
			for(int c = 0; c < ord; c++)
				rs += res[c] * (ls(r, c) = rand());
			ls(r) = rs;
		}
		Vector<double> out = ls.Solve();
		if(!out.IsEmpty())
		{
			ASSERT(out.GetCount() == ord);
			for(int c = 0; c < out.GetCount(); c++)
				if(!IsNull(out[c]))
				{
					double d = out[c] - res[c];
					ASSERT(fabs(d) <= 1e-3);
				}
		}
	}
}

}
