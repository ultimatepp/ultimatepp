template <class C, class L>
C& AppendSorted(C& dest, const C& src, const L& less)
{
	if(src.IsEmpty())
		return dest;
	if(dest.IsEmpty())
	{
		dest <<= src;
		return dest;
	}
	if(!less(*src, dest.Top()))
	{
		dest.Append(src);
		return dest;
	}
	if(!less(*dest, src.Top()))
	{
		dest.Insert(0, src);
		return dest;
	}
	int dc = dest.GetCount();
	int sc = src.GetCount();
	dest.SetCount(dc + sc);
	typename C::Iterator de = dest.End();
	typename C::ConstIterator se = src.End(), pe = dest.GetIter(dc);
	--se;
	--pe;
	for(;;)
	{
		if(less(*se, *pe))
		{
			*--de = *pe;
			if(pe == dest.Begin())
			{ // dest items are finished
				*--de = *se;
				while(se != src.Begin())
					*--de = *--se;
				return dest;
			}
			--pe;
		}
		else
		{
			*--de = *se;
			if(se == src.Begin())
				return dest; // src items are finished, dest items are in place
			--se;
		}
	}
	return dest;
}

template <class C>
C& AppendSorted(C& dest, const C& src)
{
	typedef ValueTypeOf<C> VT;
	return AppendSorted(dest, src, std::less<VT>());
}

template <class C, class L>
C& UnionSorted(C& dest, const C& src, const L& less)
{
	if(src.IsEmpty())
		return dest;
	if(dest.IsEmpty())
	{
		dest <<= src;
		return dest;
	}
	if(less(dest.Top(), *src))
	{
		dest.Append(src);
		return dest;
	}
	if(less(src.Top(), *dest))
	{
		dest.Insert(0, src);
		return dest;
	}
	int dc = dest.GetCount();
	int sc = src.GetCount();
	dest.SetCount(dc + sc);
	typename C::Iterator de = dest.End();
	typename C::ConstIterator se = src.End(), pe = dest.GetIter(dc);
	--se;
	--pe;
	for(;;)
	{
		if(less(*se, *pe))
		{
			*--de = *pe;
			if(pe == dest.Begin())
			{ // dest items are finished
				*--de = *se;
				while(se != src.Begin())
					*--de = *--se;
				dest.Remove(0, dest.GetIndex(*de));
				return dest;
			}
			--pe;
		}
		else
		{
			*--de = *se;
			if(!less(*pe, *se))
			{
				if(pe == dest.Begin())
				{
					while(se != src.Begin())
						*--de = *--se;
					dest.Remove(0, dest.GetIndex(*de));
					return dest;
				}
				--pe;
			}
			if(se == src.Begin())
			{
				int pi = (pe - dest.Begin()) + 1;
				dest.Remove(pi, (de - dest.Begin()) - pi);
				return dest; // src items are finished, dest items are in place
			}
			--se;
		}
	}
	return dest;
}

template <class C>
C& UnionSorted(C& dest, const C& src)
{
	typedef ValueTypeOf<C> VT;
	return UnionSorted(dest, src, std::less<VT>());
}

template <class C, class L>
C& RemoveSorted(C& from, const C& what, const L& less)
{
	if(from.IsEmpty() || what.IsEmpty() ||
	   less(from.Top(), *what.Begin()) || less(what.Top(), *from.Begin()))
		return from;
	typename C::ConstIterator we = what.End(), wp = BinFind(what, from[0], less);
	if(wp == we)
		return from;
	typename C::Iterator fp = from.Begin() + BinFindIndex(from, *wp), fe = from.End(), fd = fp;
	if(fp == fe)
		return from;
	for(;;)
	{
		while(less(*fp, *wp))
		{
			*fd = *fp;
			++fd;
			if(++fp == fe)
			{
				from.SetCount(fd - from.Begin());
				return from;
			}
		}
		if(less(*wp, *fp))
		{
			do
				if(++wp == we)
				{
					Copy(fd, fp, fe);
					fd += (fe - fp);
					from.SetCount(fd - from.Begin());
					return from;
				}
			while(less(*wp, *fp));
		}
		else
		{
			const ValueTypeOf<C>& value = *fp;
			while(!less(value, *fp))
				if(++fp == fe)
				{
					from.SetCount(fd - from.Begin());
					return from;
				}
			do
				if(++wp == we)
				{
					Copy(fd, fp, fe);
					fd += (fe - fp);
					from.SetCount(fd - from.Begin());
					return from;
				}
			while(!less(value, *wp));
		}
	}
}

template <class C>
C& RemoveSorted(C& from, const C& what)
{
	typedef ValueTypeOf<C> VT;
	return RemoveSorted(from, what, std::less<VT>());
}

template <class D, class S, class L>
D& IntersectSorted(D& dest, const S& src, const L& less)
{
	if(dest.IsEmpty())
		return dest;
	if(src.IsEmpty() || less(dest.Top(), src[0]) || less(src.Top(), dest[0]))
	{ // empty source set or disjunct intervals
		dest.Clear();
		return dest;
	}
	typename S::ConstIterator ss = BinFind(src, dest[0], less), se = src.End();
	if(ss == se)
	{
		dest.Clear();
		return dest;
	}
	typename D::ConstIterator ds = BinFind(dest, src[0], less), de = dest.End();
	if(ds == de)
	{
		dest.Clear();
		return dest;
	}
	typename D::Iterator d = dest.Begin();
	int count = 0;
	for(;;)
	{
		if(less(*ss, *ds))
		{
			if(++ss == se)
				break;
		}
		else
		{
			if(!less(*ds, *ss))
			{
				*d = *ds;
				++d;
				count++;
			}
			if(++ds == de)
				break;
		}
	}
	dest.SetCount(count);
	return dest;
}

template <class D, class S>
D& IntersectSorted(D& dest, const S& src)
{
	typedef ValueTypeOf<D> VT;
	return IntersectSorted(dest, src, std::less<VT>());
}
