package java.io;

import java.net.MalformedURLException;
import java.net.URI;
import java.net.URISyntaxException;
import java.net.URL;
import java.util.Collections;
import java.util.HashSet;
import java.util.Iterator;
import java.util.Random;
import java.util.Set;
import java.util.StringTokenizer;
import java.util.Vector;

/*
 * Java core library component.
 *
 * Copyright (c) 1997, 1998
 *      Transvirtual Technologies, Inc.  All rights reserved.
 *
 * See the file "license.terms" for information on usage and redistribution
 * of this file.
 */

public class File implements Serializable, Comparable {

	private static final long serialVersionUID = 301077366599181567L;
	public final static String separator = System.getProperty("file.separator");
	public final static char separatorChar = separator.charAt(0);
	public final static String pathSeparator = System.getProperty("path.separator");
	public static final char pathSeparatorChar = pathSeparator.charAt(0);

	private static final Random random = new Random();
	private final String path;
        private static DeleteOnExitHook deleteOnExitHook = new DeleteOnExitHook();

static {
	System.loadLibrary("io");
}

public File(File dir, String name) {
        this((dir == null) ? null : dir.getPath(), name);
}

public File(String path, String name) {
	/*
	 * possible npe is required by the spec.
	 */
	int i = name.length();
	
	if (i>0) {
                /*
                 * The first character of the path should never be removed,
                 * therefore >1. It's safe to do so, since we've already
                 * checked that the path is not empty.
                 */
                for (; i-->1 && name.charAt(i)==separatorChar;);
                name = name.substring(0, i+1);
	
	}
		
	if (path == null) {
	   this.path = name;
	}
	else if (path.length() == 0) {
	   this.path = separatorChar + name;
	}
	else {
		final char last = path.charAt(path.length() - 1);
		
		if (last != separatorChar) {
			this.path = path + separatorChar + name;
		}
		else {
			this.path = path + name;
		}
	}
}

public File(String path) {
        /*
         * we remove all trailing seperatorChars from the path, since
         * they are unnecessary. Note that the NullPointerException is
         * not only ok but required by the spec.
         */
        int i=path.length();

        if (i>0) {
                /*
                 * The first character of the path should never be removed,
                 * therefore >1. It's safe to do so, since we've already
                 * checked that the path is not empty.
                 */
                for (; i-->1 && path.charAt(i)==separatorChar;);
                this.path = path.substring(0, i+1);
        } else {
		this.path = "";
	}
}

public File(URI uri) {
	if (uri == null) {
		throw new NullPointerException();
	}
	this.path = uri.getPath();
	if (this.path == null) {
		throw new IllegalArgumentException();
	}
}

public boolean canRead() {
	checkReadAccess();

	return (exists() && canRead0());
}

native private boolean canRead0();

public boolean canWrite() {
	checkWriteAccess();

	return canWrite0();
}

native private boolean canWrite0();

private void checkReadAccess() {
	final SecurityManager sm = System.getSecurityManager();
	if (sm != null)
		sm.checkRead(getPath());
}

private void checkWriteAccess() {
	final SecurityManager sm = System.getSecurityManager();
	if (sm != null)
		sm.checkWrite(getPath());
}

public static File createTempFile(String prefix, String suffix)
		throws IOException {
	return createTempFile(prefix, suffix, null);
}

public static File createTempFile(String prefix, String suffix, File dir)
		throws IOException {
	if (prefix.length() < 3) {
		throw new IllegalArgumentException(prefix);
	}
	if (suffix == null) {
		suffix = ".tmp";
	}
	if (dir == null) {
		dir = new File(System.getProperties().getProperty(
			"java.io.tmpdir"));
	}
	while (true) {
		File f = new File(dir, prefix
		    + Integer.toHexString(
			random.nextInt(0x100000)).toUpperCase() + suffix);

		f.checkWriteAccess();
		if (f.createNewFile0(0600))
			return f;
	}
}

public boolean delete() {
	SecurityManager sm = System.getSecurityManager();
	if (sm != null)
		sm.checkDelete(getPath());

	return isDirectory0() ? rmdir0() : delete0();
}

native private boolean delete0();
native private boolean rmdir0();

/*
 * Free file on the termination of the application.
 */
public void deleteOnExit() {
    // Check the SecurityManager
    SecurityManager sm = System.getSecurityManager ();
    if (sm != null) {
      sm.checkDelete(path);
    }

    deleteOnExitHook.addFile(this);
}

public int compareTo(Object that) {
	return compareTo((File)that);
}

// XXX some filesystems are case-insensitive
public int compareTo(File that) {
	return this.path.compareTo(that.path);
}

// XXX some filesystems are case-insensitive
public boolean equals(Object that) {
	if (that == null || !that.getClass().equals(getClass()))
		return false;
	return path.equals(((File)that).path);
}

public boolean exists() {
	checkReadAccess();

	return exists0();
}

native private boolean exists0();

public String getAbsolutePath() {
	if (isAbsolute()) {
		return getPath();
	}
	else {
		return System.getProperty("user.dir") + separatorChar + getPath();
	}
}

public String getCanonicalPath() throws IOException {
        StringTokenizer tok = new StringTokenizer(getAbsolutePath(), separator);
        int len = tok.countTokens();
        String[] array = new String[len+2];
        int j = 0;
        for (int i = 0; i < len; i++) {
                String str = tok.nextToken();
                if (str.equals("..")) {
			if (j > 0)
				j--;
                } else if (!str.equals(".")) {
			array[j] = str;
			j++;
                }
        }

	// If the length is 0 then just return the base.
	if (j == 0) {
		return (separator);
	}

        // Build a string of the remaining elements.
        StringBuffer buf = new StringBuffer();
        for (int i = 0; i < j; i++) {
		buf.append(separatorChar);
                buf.append(array[i]);
        }
        return (buf.toString());
}

public String getName() {
	return path.substring(path.lastIndexOf(separatorChar)+1);
}

public String getParent() {
	int slashIndex = path.lastIndexOf(separatorChar);
	if (slashIndex > 0) {
		return (path.substring(0, slashIndex));
	}
	if (slashIndex == 0 && path.length() > 1) {
		return (separator);
	}
	return (null);
}

public String getPath() {
	return path;
}

public int hashCode() {
	return path.hashCode();
}

native public boolean isAbsolute();

public boolean isDirectory() {
	checkReadAccess();

	return isDirectory0();
}

native private boolean isDirectory0();

public boolean isFile() {
	checkReadAccess();

	return isFile0();
}

native private boolean isFile0();

public long lastModified() {
	checkReadAccess();

	return lastModified0();
}

native private long lastModified0();

public long length() {
	checkReadAccess();

	return length0();
}

native private long length0();

public String[] list() {
	checkReadAccess();

	return list0();
}

public String[] list(FilenameFilter filter) {
	String all[] = list();

	if ((all == null) || (filter == null) ) {
		return (all);
	}

	Vector filtered = new Vector();

	for (int idx = 0; idx < all.length; idx++) {
		String fn = all[idx];
		if (filter.accept(this, fn)) {
			filtered.addElement(fn);
		}
	}

	String result[] = new String[filtered.size()];
	filtered.copyInto(result);

	return (result);
}

public File[] listFiles() {
	return listFiles((FilenameFilter)null);
}

public File[] listFiles(final FileFilter filter) {
	if (filter == null) {
		return listFiles ((FilenameFilter)null);
	} else {
		return listFiles(new FilenameFilter() {
	  	  public boolean accept(File dir, String name) {
	    	    return filter.accept(new File(dir, name));
	  	  }
		});
	}
}

public File[] listFiles(FilenameFilter filter) {
	String[] names = list(filter);
	if (names == null) {
		return null;
	}
	File[] files = new File[names.length];
	for (int idx = 0; idx < names.length; idx++) {
		files[idx] = new File(this, names[idx]);
	}
	return files;
}

native private String[] list0();

public boolean mkdir() {
	checkWriteAccess();

	if (isDirectory()) {
		return (true);
	}
	else {
		return (mkdir0());
	}
}

native private boolean mkdir0();

public boolean mkdirs() {
	if (!toString().equals(separator)) {
		String parentString = getParent();
		if (parentString != null) {
			File parent = new File(parentString);
			if (!parent.mkdirs()) {
				return (false);
			}
		}
	}
	return (mkdir());
}

public boolean renameTo(File dest) {
	SecurityManager sm = System.getSecurityManager();
	if (sm != null) {
		sm.checkWrite(getPath());
		sm.checkWrite(dest.getPath());
	}

	return renameTo0(dest);
}

native private boolean renameTo0(File that);

public String toString() {
	return path;
}

public URL toURL() throws MalformedURLException {
	return new URL("file", "",
	    isDirectory() ? getAbsolutePath() + separator : getAbsolutePath());
}

public boolean isHidden() {
	checkReadAccess();
	return getName().startsWith(".");
}

public File getAbsoluteFile() {
	return new File(getAbsolutePath());
}

public File getParentFile() {
	String p = getParent();
	return (p == null) ? null : new File(p);
}

public File getCanonicalFile() throws IOException {
	return new File(getCanonicalPath());
}

public boolean createNewFile() throws IOException {
	checkWriteAccess();
	return createNewFile0(0666);
}

native private boolean createNewFile0(int mode) throws IOException;

public boolean setLastModified(long time) {
	checkWriteAccess();
	return setLastModified0(time);
}

native private boolean setLastModified0(long time);

public boolean setReadOnly() {
	checkWriteAccess();
	return setReadOnly0();
}

native private boolean setReadOnly0();

public static File[] listRoots() {
	// XXX FIXME: incorrect for Windows platforms, eg: { "C:", "D:" }
	return new File[] { new File(separator) };
}

/**
 * @since 1.4
 */
public URI toURI() {
	try {
		return new URI("file",
			       null,
			       (isDirectory() ?                               
				getAbsolutePath() + separator
				: getAbsolutePath()),
			       null,
			       null);
	}
	catch (URISyntaxException e) {
		throw (IllegalArgumentException) 
			new IllegalArgumentException("Couldn't convert "
						     + toString()
						     + " to an URI")
			.initCause(e);
	}
}

    private static class DeleteOnExitHook extends Thread {

	private Set files = Collections.synchronizedSet(new HashSet());

	public DeleteOnExitHook() {
	}

	public void addFile(File file) {
	    files.add(file);
	}

	public void run() {
	    Iterator iter = files.iterator();

	    while(iter.hasNext()) {
		File file = (File) iter.next();
		file.delete();
	    }
	}
    }
}
