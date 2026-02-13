using System;
using System.IO;

class Program
{
    static void Main()
    {
        string targetDirectory = @"../src";
        string comment = "//This code was made for the Multimedia Project 2a for the Multimedia Technology class at Fachhochschule Salzburg, by Pascal Veselka & Pascal Marsche\n";
        string trigger = "//This code was made for the Multimedia Project 2a for the Multimedia Technology class at Fachhochschule Salzburg, by Pascal Veselka & Pascal Marsche\n";

        string[] extensions = { ".cpp", ".hpp", ".h" };

        try
        {
            // Get all files, then filter by the extensions list
            var files = Directory.EnumerateFiles(targetDirectory, "*.*", SearchOption.AllDirectories)
                                 .Where(f => extensions.Contains(Path.GetExtension(f).ToLower()));

            foreach (string filePath in files)
            {
                ProcessFile(filePath, comment, trigger);
            }
        }
        catch (Exception ex)
        {
            Console.WriteLine($"Error accessing directory: {ex.Message}");
        }
    }
    static void ProcessFile(string path, string comment, string trigger)
    {
        try
        {
            // 1. Read all lines into a list
            var lines = File.ReadAllLines(path).ToList();

            if (lines.Count > 0)
            {
                // 2. Check the first line (ignoring leading whitespace)
                if (lines[0].TrimStart().StartsWith(trigger))
                {
                    Console.WriteLine($"Trigger found in {Path.GetFileName(path)}. Replacing line...");

                    // 3. Remove the old line and insert the new one
                    lines.RemoveAt(0);
                    lines.Insert(0, comment.TrimEnd()); // TrimEnd to avoid double newlines

                    // 4. Save the changes
                    File.WriteAllLines(path, lines);
                   Console.WriteLine($"Updated with new Comment: {Path.GetFileName(path)}");
                }
                else
                {
                    string content = File.ReadAllText(path);
                    File.WriteAllText(path, comment + content);
                    Console.WriteLine($"Updated: {Path.GetFileName(path)}");
                }
            }
        }
        catch (IOException ex)
        {
            Console.WriteLine($"Error processing {path}: {ex.Message}");
        }
    }
}