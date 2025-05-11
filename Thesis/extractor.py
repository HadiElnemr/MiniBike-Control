import re, os, subprocess

INPUT_TEX = "chapters/2-Theory.tex"
OUTPUT_DIR = "svg_equations"

# Create output directory
os.makedirs(OUTPUT_DIR, exist_ok=True)

with open(INPUT_TEX, "r", encoding="utf-8") as f:
    tex = f.read()

# Find all display equations
matches = re.findall(
    r'\\begin\{(subequations\*?|equation\*?|equation|align\*?|align)\}(.*?)\\end\{\1\}',
    tex, re.DOTALL
)

for i, match in enumerate(matches, 1):
    env_name = match[0]  # e.g., 'equation'
    eq_content = match[1].strip()
    eq_content = re.sub(r'\\label\{.*?\}', '', eq_content)

    eq_tex = f"""
\\documentclass[preview,border=2pt]{{standalone}}
\\usepackage{{amsmath,amssymb}}
\\begin{{document}}
\\begin{{{env_name}}}
{eq_content}
\\end{{{env_name}}}
\\end{{document}}
"""
    tex_filename = os.path.join(OUTPUT_DIR, f"eq_{i}.tex")
    pdf_filename = os.path.join(OUTPUT_DIR, f"eq_{i}.pdf")
    svg_filename = os.path.join(OUTPUT_DIR, f"eq_{i}.svg")

    with open(tex_filename, "w", encoding="utf-8") as f:
        f.write(eq_tex)

    # Compile to PDF
    print("pdflatex", "-output-directory", OUTPUT_DIR, tex_filename)
    # subprocess.run(["pdflatex", "-output-directory", OUTPUT_DIR, tex_filename], stdout=subprocess.DEVNULL)

    # # Convert PDF to SVG
    # subprocess.run(["pdf2svg", pdf_filename, svg_filename])
